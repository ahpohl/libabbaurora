#include <iostream>
#include <string>
#include <iomanip>
#include <cstring>
#include <termios.h>    // contains POSIX terminal control definition
#include <fcntl.h>      // contains file controls like 0_RDWR
#include <unistd.h>     // write(), read(), close()
#include <errno.h>      // error integer and strerror() function
#include <sys/ioctl.h>  // contains ioctl_tty

ABBAuroraSerial::~ABBAuroraSerial
{
  if (SerialPort > 0) {
    close(m_serialport);
  }
}

void ABBAuroraSerial::begin(std::string &device)
{
  if (device.empty()) {
    throw runtime_error("Serial device argument empty");
  }
 
  SerialPort = open(device.c.str(), O_RDWR | O_NOCTTY);
  if (SerialPort < 0) {
    throw runtime_error(string("Error opening device ") + device + ": "
      + strerror(errno) + " (" + to_string(errno) + ")");
  }

  int ret = ioctl(SerialPort, TIOCEXCL);
  if (ret < 0) {
    throw runtime_error(string("Error getting device lock on") 
      + device + ": " + strerror(errno) + " (" + to_string(errno) + ")");
  }

  struct termios attr;

  memset(&attr, 0, sizeof(attr));
  int ret = tcgetattr(SerialPort, &attr);
  if (ret) {
    throw runtime_error(string("Error getting serial port attributes: ")
      + strerror(errno) + " (" + to_string(errno) + ")");
  }

  cfmakeraw(&attr);

  // configure serial port
  // speed: 19200 baud, data bits: 8, stop bits: 1, parity: no
  attr.c_cflag &= ~CSIZE;
  attr.c_cflag |= CS8;
  cfsetispeed(&attr, B19200);
  cfsetospeed(&attr, B19200);

  // set vmin and vtime for blocking read
  // vmin: returning when max 1 byte is available
  // vtime: wait for up to 0.1 second between characters
  attr.c_cc[VMIN] = t_vmin;
  attr.c_cc[VTIME] = t_vtime;
  
  ret = tcsetattr(SerialPort, TCSANOW, &attr);
  if (ret != 0) {
    throw runtime_error(string("Error setting serial port attributes: ") 
      + strerror(errno) + " (" + to_string(errno) + ")");
  }
}

int ABBAuroraSerial::readBytes(uint8_t *buffer, size_t &length)
{
}

int ABBAuroraSerial::write(uint8_t *buffer, size_t &length)
{
}

void ABBAuroraSerial::flush(void)
{
}
