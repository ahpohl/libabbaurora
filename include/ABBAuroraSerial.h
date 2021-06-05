#ifndef ABBAuroraSerial_h
#define ABBAuroraSerial_h
#include <string>
#include <termios.h>

/** @brief Low level serial methods

    Methods to handle the serial communication between host and device

    */
class ABBAuroraSerial
{
public:
/** @brief Default destructor

    Closes the serial port and deletes the serial object

    */
  ~ABBAuroraSerial(void);
/** @brief Begin communication

    Opens the serial port and sets communication parameters

    @param device The serial device, i.e. /dev/ttyUSB0
    @param baudrate The serial baud rate
    */
  bool Begin(const std::string &device, const speed_t &baudrate);
/** @brief Read bytes

    Read bytes available in the input buffer

    @param buffer Buffer to store the bytes
    @param length Number of bytes to read 
    */
  int ReadBytes(uint8_t *buffer, const int &length);
/** @brief Write bytes
  
    Transmit bytes to the device

    @param buffer Buffer with bytes to send
    @param length Number of bytes to send
    */
  int WriteBytes(uint8_t const *buffer, const int &length);
/** @brief Flush buffers
  
    Flushes/discards data which is written but not transmitted and/or data which is received but not read by the user

    */
  void Flush(void) const;
/** @brief Return word
  
    Construct a word (2 bytes) from two single bytes   

    @param msb Most significant (left) byte
    @param lsb Least significant (right) byte
    */
  uint16_t Word(const uint8_t &msb, const uint8_t &lsb) const;
/** @brief Calculate checksum
  
    Calculate a CCITT CRC 16 checksum  

    @param data Buffer with data
    @param offset Start calucation at an offset
    @param count Number of bytes to include
    */
  uint16_t Crc16(uint8_t *data, const int &offset, const int &count) const;
/** @brief Return low byte
  
    Return the least significant (low) byte of a word

    @param bytes Word to convert
    */
  uint8_t LowByte(const uint16_t &bytes) const;
/** @brief Return high byte
  
    Return the most significant (high) byte of a word

    @param bytes Word to convert
    */ 
  uint8_t HighByte(const uint16_t &bytes) const;
/** @brief Get error message
  
    Returns the error message of a failure

    */
  std::string GetErrorMessage(void);

private:
  int SerialPort; ///< Serial port number
  std::string ErrorMessage; ///< Error message string
};

#endif
