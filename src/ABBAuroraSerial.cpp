#include <iostream>
#include <string>
#include <iomanip>
#include <cstring>
#include <termios.h>    // contains POSIX terminal control definition
#include <fcntl.h>      // contains file controls like 0_RDWR
#include <unistd.h>     // write(), read(), close()
#include <errno.h>      // error integer and strerror() function
#include <sys/ioctl.h>  // contains ioctl_tty
#include "ABBAuroraSerial.h"

ABBAuroraSerial::~ABBAuroraSerial(void)
{
  if (SerialPort > 0) {
    close(SerialPort);
  }
}

void ABBAuroraSerial::begin(std::string device)
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

  // set vmin and vtime for blocking read
  // vmin: returning when max 8 bytes are available
  // vtime: wait for up to 0.1 second between characters
  serial_port_settings.c_cc[VMIN] = 8;
  serial_port_settings.c_cc[VTIME] = 1;
  
  ret = tcsetattr(SerialPort, TCSANOW, &serial_port_settings);
  if (ret != 0) {
    throw std::runtime_error(std::string("Error setting serial port attributes: ") 
      + strerror(errno) + " (" + std::to_string(errno) + ")");
  }
  tcflush(SerialPort, TCIOFLUSH);
}

int ABBAuroraSerial::readBytes(uint8_t *buffer, size_t length)
{
  int bytesReceived = 0;

  bytesReceived = read(SerialPort, buffer, length);

  return bytesReceived;
}

int ABBAuroraSerial::writeBytes(uint8_t const *buffer, size_t length)
{
  int bytesSent = 0;

  bytesSent = write(SerialPort, buffer, length);
  tcdrain(SerialPort);

  return bytesSent;
}

void ABBAuroraSerial::flush(void)
{
  tcflush(SerialPort, TCIOFLUSH);
}
