#ifndef ABBAurora_h
#define ABBAurora_h
#include <string>
#include "ABBAuroraEnums.h"
#include "ABBAuroraSerial.h"

/** @brief Communication protocol between host and supervisor microprocessor 
 
    The communication between host and processor works via a serial interface RS485 or RS232.

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
  BaudCodeEnum BaudCode;
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
/** @name General methods
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
/** @brief Set serial device address

    Sets a new RS485 serial device address     

    @param addr RS485 device address, range 2-63 
    */
  void SetAddress(const unsigned char &addr);
/** @brief Get serial device address

    Gets the current RS485 serial device address     
    */
  unsigned char GetAddress(void) const;
/** @brief Get error message

    Returns the error message string
    */
  std::string GetErrorMessage(void) const;
///@}


/** @name Description of commands
    */
///@{

/** @brief State request

    Ask the state of the system modules
    */
  bool ReadState(void);
/** @brief Version reading

    Reads the version (Indoor/Outdoor, Europe/America, available only for FW version 1.0.9 and following)
    */
  bool ReadVersion(void);
/** @brief Measure request to the DSP

    Sends a request to measure voltage, current, power etc.

    The value is expressed in the following measurements units:
    - Voltages [V]
    - Currents [A]
    - Powers [W]
    - Temperatures [°C]

    @param type Requested value, described in [Ref DspValueEnum]
    @param global Measurement global or local, described in [Ref DspGlobalEnum]
    */ 
  bool ReadDspValue(const DspValueEnum &type, const DspGlobalEnum &global);
/** @brief Time/Date reading

    Reads the time and date from the inverter, with one second accuracy
    */
  bool ReadTimeDate(void); 
/** @brief Last four alarms

    The command returns the codes of the last four alarms in form of a FIFO queue from the first to the last one. When this command is used the queue is emptied. Alarm codes are described in [ref to State request]
    */
  bool ReadLastFourAlarms(void);
/** @brief Baud rate setting on serial lines

    Sets the baud rate of the serial line(s). The serial-line field is reserved to Aurora Central modules.

    @param baudcode Baud rate setting described in [ref BaudCodeEnum]
    @param serialline Number of the serial line, range 1 (external lines), 2, ..., 255
    */
  bool WriteBaudRateSetting(const BaudCodeEnum &baudcode, const unsigned char &serialline);

  /// Data structure for state request command
  struct State
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    unsigned char InverterState;
    unsigned char Channel1State;
    unsigned char Channel2State;
    unsigned char AlarmState;
  } State;
  /// Data structure for version reading command
  struct Version
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    std::string Par1;
    std::string Par2;
    std::string Par3;
    std::string Par4;
  } Version;
  /// Data structure for measure request to dsp command
  struct Dsp
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    float Value;
  } Dsp;
  /// Data structure for time/date reading command
  struct TimeDate
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    unsigned long Seconds;
  } TimeDate;
  /// Data structure for last four alarms command
  struct LastFourAlarms
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    unsigned char Alarms1;
    unsigned char Alarms2;
    unsigned char Alarms3;
    unsigned char Alarms4;
  } LastFourAlarms;
///@}


/** @name Inverter commands

    This section described commands which are only applicable to Aurora inverters
    */
///@{

/** @brief P/N reading

    Reads the system part number (Aurora inverters). No information on transmission and global state is returned.
    */
  bool ReadSystemPN(void);
/** @brief Serial number reading

    Reads the system serial number (Aurora inverters). No information on transmission and global state is returned.
    */
  bool ReadSystemSerialNumber(void);
/** @brief Manufacturing date reading

    Reads the Manufacturing week and year (Aurora inverters)
    */
  bool ReadManufacturingWeekYear(void);
/** @brief Firmware release reading

    Reads the firmware release. For Aurora grid-tied inverters you will read always the MCU firmware version (the field var is not interpreted)
    */
  bool ReadFirmwareRelease(void);
/** @brief Cumulated energy readings

    Reads the cumumlated energy (Aurora grid-tied inverters only)

    @param period Energy period described in [ref CumulatedEnergyEnum]
    */
  bool ReadCumulatedEnergy(const CumulatedEnergyEnum &period);

/// String for the read system P/N command
  std::string SystemPN;
/// String for the read serial number command
  std::string SystemSerialNumber;
/// Data structure for the read manufacturing date command
  struct ManufacturingWeekYear
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    std::string Week;
    std::string Year;
  } ManufacturingWeekYear;
/// Data structure for the read firmware release command
  struct FirmwareRelease
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    std::string Release;
  } FirmwareRelease;
/// Data structure for the read cumulated energy command
  struct CumulatedEnergy
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    unsigned long Energy;
  } CumulatedEnergy;
///@}
};

#endif
