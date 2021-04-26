#ifndef ABBAuroraSerial_h
#define ABBAuroraSerial_h
#include <string>

class ABBAuroraSerial
{
public:
  ~ABBAuroraSerial(void);
  void begin(std::string device);
  int readBytes(uint8_t *buffer, int length);
  int writeBytes(uint8_t const *buffer, int length);
  void flush(void);

private:
  int SerialPort;
};

#endif
