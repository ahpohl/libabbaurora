#ifndef ABBAuroraSerial_h
#define ABBAuroraSerial_h
#include <string>

class ABBAuroraSerial
{
public:
  ~ABBAuroraSerial(void);
  void Begin(std::string device);
  int ReadBytes(uint8_t *buffer, int length);
  int WriteBytes(uint8_t const *buffer, int length);
  void Flush(void);

private:
  int SerialPort;
};

#endif
