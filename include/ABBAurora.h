#ifndef ABBAurora_h
#define ABBAurora_h
#include <string>
#include "ABBAuroraEnums.h"
#include "ABBAuroraSerial.h"

/** @brief Communication protocol between host and supervisor microprocessor 
 
    The communication between host and processor works via a serial interface RS485 or RS232. Many devices can be chained together

    Configuration parameters in both cases are:
    - 19200 baud (default value)
    - 1 stop bit
    - no parity

    If the device has a RS485 interface, the default serial bus address is 2.

    The communication protocol uses fixed length transmission messages (8 bytes + 2 bytes for checksum). The structure of the answer also has fixed length (6 bytes + 2 bytes for checksum).

    @author Alexander Pohl <alex@ahpohl.com>
    */
class ABBAurora
{
private:
  static const int SEND_BUFFER_SIZE;
  static const int RECEIVE_BUFFER_SIZE;
  static const SendCommandEnum SEND_COMMAND;

  unsigned char Address;
  uint8_t *ReceiveData;
  ABBAuroraSerial *Serial;
  speed_t GetBaudRate(const BaudCodeEnum &baudcode) const;

  bool Send(SendCommandEnum cmd, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7);

  union Flo
  {
    unsigned char asBytes[4];
    float asFloat;
  } Flo;

  union Ulo
  {
    unsigned char asBytes[4];
    unsigned long asUlong;
  } Ulo;

  std::string ErrorMessage;

public:
/** @name Communication protocol
      
    Methods for both Aurora inverters and Aurora central
    */
///@{

/** @brief Default class constructor
      
    Initialises the class object with the default bus address and baud rate:
    */
  ABBAurora(void);
/** @brief Overloaded class constructor
      
    Initialises the class object with the default baud rate.

    @param addr RS485 device address, range 2-63
    */
  ABBAurora(const unsigned char &addr);
/** @brief Overloaded class constructor
      
    Initialises the class object.

    @param addr RS485 device address, range 2-63
    @param baud Baud rate setting, reference to BaudCodeEnum.
    */
  ABBAurora(const unsigned char &addr, const BaudCodeEnum &baud);
/** @brief Default class destructor
      
    Closes the serial port and destroys the class object.
    */
  ~ABBAurora(void);
/** @brief Setup serial device communication
      
    Opens the host serial device and sets the communication parameters

    @param device The serial device, i.e. /dev/ttyUSB0
    */
  bool Setup(const std::string &device);

  ///@}

  void SetAddress(const unsigned char &addr);
  unsigned char GetAddress(void) const;

  std::string GetErrorMessage(void) const;

  struct State
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    unsigned char InverterState;
    unsigned char Channel1State;
    unsigned char Channel2State;
    unsigned char AlarmState;
  } State;

  bool ReadState(void);

  struct Version
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    std::string Par1;
    std::string Par2;
    std::string Par3;
    std::string Par4;
  } Version;

  bool ReadVersion(void);

  struct Dsp
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    float Value;
  } Dsp;

  bool ReadDspValue(const DspValueEnum &type, const DspGlobalEnum &global);

  struct TimeDate
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    unsigned long Seconds;
  } TimeDate;

  bool ReadTimeDate(void);

  struct LastFourAlarms
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    unsigned char Alarms1;
    unsigned char Alarms2;
    unsigned char Alarms3;
    unsigned char Alarms4;
  } LastFourAlarms;

  bool ReadLastFourAlarms(void);

  bool ReadJunctionBoxState(const unsigned char &nj);

  bool ReadJunctionBoxVal(const unsigned char &nj, const unsigned char &par);

  // Inverters
  std::string SystemPN;
  bool ReadSystemPN(void);

  std::string SystemSerialNumber;
  bool ReadSystemSerialNumber(void);

  struct ManufacturingWeekYear
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    std::string Week;
    std::string Year;
  } ManufacturingWeekYear;

  bool ReadManufacturingWeekYear(void);

  struct FirmwareRelease
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    std::string Release;
  } FirmwareRelease;

  bool ReadFirmwareRelease(void);

  struct CumulatedEnergy
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    unsigned long Energy;
  } CumulatedEnergy;

  bool ReadCumulatedEnergy(const CumulatedEnergyEnum &par);

  bool WriteBaudRateSetting(const BaudCodeEnum &baudcode);

  // Central
  BaudCodeEnum BaudCode;

  bool ReadFlagsSwitchCentral(void);

  bool ReadCumulatedEnergyCentral(const unsigned char &var, const unsigned char &ndays_h, const unsigned char &ndays_l, const unsigned char &global);

  bool ReadFirmwareReleaseCentral(const unsigned char &var);

  bool ReadBaudRateSettingCentral(const BaudCodeEnum &baudcode, const unsigned char &serialline);

  bool ReadSystemInfoCentral(const unsigned char &var);

  bool ReadJunctionBoxMonitoringCentral(const unsigned char &cf, const unsigned char &rn, const unsigned char &njt, const unsigned char &jal, const unsigned char &jah);

  bool ReadSystemPNCentral(void);

  bool ReadSystemSerialNumberCentral(void);

};

#endif
