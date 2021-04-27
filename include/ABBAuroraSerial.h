#ifndef ABBAuroraSerial_h
#define ABBAuroraSerial_h
#include <string>

class ABBAuroraSerial
{
public:
  ~ABBAuroraSerial(void);
  void Begin(const std::string &device);
  int ReadBytes(uint8_t *buffer, const int &length) const;
  int WriteBytes(uint8_t const *buffer, const int &length) const;
  void Flush(void) const;
  uint16_t Word(const uint8_t &msb, const uint8_t &lsb) const;
  uint16_t Crc16(uint8_t *data, const int &offset, const int &count) const;
  uint8_t LowByte(const uint16_t &bytes) const;
  uint8_t HighByte(const uint16_t &bytes) const;

private:
  int SerialPort;
};

#endif
