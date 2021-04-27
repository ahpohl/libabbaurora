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

void ABBAuroraSerial::Begin(std::string device)
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
  cfsetispeed(&serial_port_settings, B19200);
  cfsetospeed(&serial_port_settings, B19200);

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

int ABBAuroraSerial::ReadBytes(uint8_t *buffer, int max_bytes_to_read)
{
  int bytesReceived, retval = 0;
  int count = 0;
  //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(); 
 
  while (count < 1000) {
    int bytes_available;
    retval = ioctl(SerialPort, FIONREAD, &bytes_available);
    if (retval < 0) {
      throw std::runtime_error("FIONREAD ioctl failed");
    }
    // delay: 1 / baud_rate * 10e6 * max_bytes_to_read = 416 µs
    std::this_thread::sleep_for(std::chrono::microseconds(500));
    if (bytes_available >= max_bytes_to_read)
      break;
    count++;
  }
  //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
  //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
  //std::cout << "Iterations: " << count << std::endl;

  bytesReceived = read(SerialPort, buffer, max_bytes_to_read);
  if (bytesReceived < 0) {
    throw std::runtime_error("Read on SERIAL_DEVICE failed");
  }
  
  return bytesReceived;
}

int ABBAuroraSerial::WriteBytes(uint8_t const *buffer, int length)
{
  int bytesSent = 0;

  bytesSent = write(SerialPort, buffer, length);
  if (bytesSent < 0) {
    throw std::runtime_error("Write on SERIAL_DEVICE failed");
  }
  tcdrain(SerialPort);

  return bytesSent;
}

void ABBAuroraSerial::Flush(void)
{
  tcflush(SerialPort, TCIOFLUSH);
}

uint16_t ABBAuroraSerial::Word(uint8_t msb, uint8_t lsb)
{
  return ((msb & 0xFF) << 8) | lsb;
}

uint16_t ABBAuroraSerial::Crc16(uint8_t *data, int offset, int count)
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

uint8_t ABBAuroraSerial::LowByte(uint16_t bytes)
{
  return static_cast<uint8_t>(bytes);
}

uint8_t ABBAuroraSerial::HighByte(uint16_t bytes)
{
  return static_cast<uint8_t>((bytes >> 8) & 0xFF);
}
