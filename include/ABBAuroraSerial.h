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
  uint16_t Crc16(uint8_t *data, int offset, int count);
  uint16_t Word(uint8_t msb, uint8_t lsb);
  uint8_t LowByte(uint16_t bytes);
  uint8_t HighByte(uint16_t bytes);

private:
  int SerialPort;
};

#endif
