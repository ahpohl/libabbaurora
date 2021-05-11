#include <cstring>
#include <iostream>
#include <thread>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "ABBAuroraSerial.h"

ABBAuroraSerial::~ABBAuroraSerial(void)
{
  if (SerialPort) {
    close(SerialPort);
  }
}

bool ABBAuroraSerial::Begin(const std::string &device, const speed_t &baudrate)
{
  if (device.empty()) {
    ErrorMessage = "Serial device argument empty";
    return false;
  }
 
  SerialPort = open(device.c_str(), O_RDWR | O_NOCTTY);
  if (SerialPort < 0) {
    ErrorMessage = std::string("Error opening device ") + device + ": "
      + strerror(errno) + " (" + std::to_string(errno) + ")";
    return false;
  }

  int ret = ioctl(SerialPort, TIOCEXCL);
  if (ret < 0) {
    ErrorMessage = std::string("Error getting device lock on") 
      + device + ": " + strerror(errno) + " (" + std::to_string(errno) + ")";
    return false;
  }

  struct termios serial_port_settings;

  memset(&serial_port_settings, 0, sizeof(serial_port_settings));
  ret = tcgetattr(SerialPort, &serial_port_settings);
  if (ret) {
    ErrorMessage = std::string("Error getting serial port attributes: ")
      + strerror(errno) + " (" + std::to_string(errno) + ")";
    return false;
  }

  cfmakeraw(&serial_port_settings);

  // configure serial port
  // speed: 19200 baud, data bits: 8, stop bits: 1, parity: no
  cfsetispeed(&serial_port_settings, baudrate);
  cfsetospeed(&serial_port_settings, baudrate);

  // set vmin and vtime for non-blocking read
  // vmin: read() returns when x byte(s) are available
  // vtime: wait for up to x * 0.1 second between characters
  serial_port_settings.c_cc[VMIN] = 0;
  serial_port_settings.c_cc[VTIME] = 0;

  ret = tcsetattr(SerialPort, TCSANOW, &serial_port_settings);
  if (ret != 0) {
    ErrorMessage = std::string("Error setting serial port attributes: ") 
      + strerror(errno) + " (" + std::to_string(errno) + ")";
    return false;
  }
  tcflush(SerialPort, TCIOFLUSH);

  return true;
}

int ABBAuroraSerial::ReadBytes(uint8_t *buffer, const int &length)
{
  int bytes_received, retval, iterations = 0;
  const int max_iterations = 1000;
  //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(); 
 
  while (iterations < max_iterations) {
    int bytes_available;
    retval = ioctl(SerialPort, FIONREAD, &bytes_available);
    if (retval < 0) {
      ErrorMessage = "Serial FIONREAD ioctl failed";
      return -1;
    }
    // intercharacter delay: 1 / baud rate * 1e6 = 52 µs
    std::this_thread::sleep_for(std::chrono::microseconds(50));
    if (bytes_available >= length)
      break;
    iterations++;
  }
  //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
  //std::cout << "Iterations: " << iterations << std::endl;

  if (iterations == max_iterations)
  {
    ErrorMessage = "Timeout, inverter could not be reached";
    return -1;
  }

  bytes_received = read(SerialPort, buffer, length);
  if (bytes_received < 0) {
    ErrorMessage = "Read on serial device failed";
    return -1;
  }

  return bytes_received;
}

int ABBAuroraSerial::WriteBytes(uint8_t const *buffer, const int &length)
{
  int bytes_sent = 0;

  bytes_sent = write(SerialPort, buffer, length);
  if (bytes_sent < 0) {
    ErrorMessage = "Write on serial device failed";
    return -1;
  }
  tcdrain(SerialPort);

  return bytes_sent;
}

void ABBAuroraSerial::Flush(void) const
{
  tcflush(SerialPort, TCIOFLUSH);
}

uint16_t ABBAuroraSerial::Word(const uint8_t &msb, const uint8_t &lsb) const
{
  return ((msb & 0xFF) << 8) | lsb;
}

uint16_t ABBAuroraSerial::Crc16(uint8_t *data, const int &offset, const int &count) const
{
  uint8_t BccLo = 0xFF;
  uint8_t BccHi = 0xFF;

  for (int i = offset; i < (offset + count); i++)
  {
    uint8_t New = data[offset + i] ^ BccLo;
    uint8_t Tmp = New << 4;
    New = Tmp ^ New;
    Tmp = New >> 5;
    BccLo = BccHi;
    BccHi = New ^ Tmp;
    Tmp = New << 3;
    BccLo = BccLo ^ Tmp;
    Tmp = New >> 4;
    BccLo = BccLo ^ Tmp;
  }

  return Word(~BccHi, ~BccLo);
}

uint8_t ABBAuroraSerial::LowByte(const uint16_t &bytes) const
{
  return static_cast<uint8_t>(bytes);
}

uint8_t ABBAuroraSerial::HighByte(const uint16_t &bytes) const
{
  return static_cast<uint8_t>((bytes >> 8) & 0xFF);
}

std::string ABBAuroraSerial::GetErrorMessage(void)
{
  return ErrorMessage;
}
