#ifndef ABBAuroraSerial_h
#define ABBAuroraSerial_h
#include <string>

class ABBAuroraSerial
{
public:
  ~ABBAuroraSerial(void);
  void begin(std::string &device);
  int readBytes(uint8_t *buffer, size_t &length);
  int write(uint8_t *buffer, size_t &length);
  void flush(void);

private:
  int SerialPort;
};

#endif
