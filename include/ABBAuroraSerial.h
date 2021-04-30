#ifndef ABBAuroraSerial_h
#define ABBAuroraSerial_h
#include <string>
#include <termios.h>

class ABBAuroraSerial
{
public:
  ~ABBAuroraSerial(void);
  bool Begin(const std::string &device, const speed_t &baudrate);
  int ReadBytes(uint8_t *buffer, const int &length);
  int WriteBytes(uint8_t const *buffer, const int &length);
  void Flush(void) const;
  uint16_t Word(const uint8_t &msb, const uint8_t &lsb) const;
  uint16_t Crc16(uint8_t *data, const int &offset, const int &count) const;
  uint8_t LowByte(const uint16_t &bytes) const;
  uint8_t HighByte(const uint16_t &bytes) const;
  std::string GetErrorMessage(void);

private:
  int SerialPort;
  std::string ErrorMessage;
};

#endif
