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
  if (SerialPort > 0) {
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
  
  while (true) {
    int bytes_available;
    retval = ioctl(SerialPort, FIONREAD, &bytes_available);
    if (retval < 0) {
      throw std::runtime_error("FIONREAD ioctl failed");
    }
    std::this_thread::sleep_for(std::chrono::microseconds(500));
    if (bytes_available >= max_bytes_to_read)
      break;
  }
  
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
