#include <iostream>
#include <string>
#include <iomanip>
#include <cstring>
#include <chrono>
#include <thread>
#include <termios.h>    // contains POSIX terminal control definition
#include <fcntl.h>      // contains file controls like 0_RDWR
#include <unistd.h>     // write(), read(), close()
#include <errno.h>      // error integer and strerror() function
#include <sys/ioctl.h>  // contains ioctl_tty
#include "ABBAuroraSerial.h"

ABBAuroraSerial::~ABBAuroraSerial(void)
{
  if (SerialPort) {
    close(SerialPort);
  }
}

void ABBAuroraSerial::Begin(const std::string &device, const speed_t &baudrate)
{
  if (device.empty()) {
    throw std::runtime_error("Serial device argument empty");
  }
 
  SerialPort = open(device.c_str(), O_RDWR | O_NOCTTY);
  if (SerialPort < 0) {
    throw std::runtime_error(std::string("Error opening device ") + device + ": "
      + strerror(errno) + " (" + std::to_string(errno) + ")");
  }

  int ret = ioctl(SerialPort, TIOCEXCL);
  if (ret < 0) {
    throw std::runtime_error(std::string("Error getting device lock on") 
      + device + ": " + strerror(errno) + " (" + std::to_string(errno) + ")");
  }

  struct termios serial_port_settings;

  memset(&serial_port_settings, 0, sizeof(serial_port_settings));
  ret = tcgetattr(SerialPort, &serial_port_settings);
  if (ret) {
    throw std::runtime_error(std::string("Error getting serial port attributes: ")
      + strerror(errno) + " (" + std::to_string(errno) + ")");
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
    throw std::runtime_error(std::string("Error setting serial port attributes: ") 
      + strerror(errno) + " (" + std::to_string(errno) + ")");
  }
  tcflush(SerialPort, TCIOFLUSH);
}

int ABBAuroraSerial::ReadBytes(uint8_t *buffer, const int &length) const
{
  int bytes_received, retval, iterations = 0;
  //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(); 
 
  while (iterations < 1000) {
    int bytes_available;
    retval = ioctl(SerialPort, FIONREAD, &bytes_available);
    if (retval < 0) {
      throw std::runtime_error("FIONREAD ioctl failed");
    }
    // delay: 1 / baud_rate * 10e6 * max_bytes_to_read = 416 µs
    std::this_thread::sleep_for(std::chrono::microseconds(500));
    if (bytes_available >= length)
      break;
    iterations++;
  }
  //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
  //std::cout << "Iterations: " << iterations << std::endl;

  bytes_received = read(SerialPort, buffer, length);
  if (bytes_received < 0) {
    throw std::runtime_error("Read on SERIAL_DEVICE failed");
  }
  
  return bytes_received;
}

int ABBAuroraSerial::WriteBytes(uint8_t const *buffer, const int &length) const
{
  int bytes_sent = 0;

  bytes_sent = write(SerialPort, buffer, length);
  if (bytes_sent < 0) {
    throw std::runtime_error("Write on SERIAL_DEVICE failed");
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
