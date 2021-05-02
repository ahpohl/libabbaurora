#ifndef ABBAurora_h
#define ABBAurora_h
#include <memory>
#include "ABBAuroraEnums.h"
#include "ABBAuroraSerial.h"

/** @brief Communication protocol between host and supervisor microprocessor 
 
    The communication between host and processor works via a serial interface RS485 or RS232.

    Configuration parameters in both cases are:
    - 19200 baud (default value)
    - 1 stop bit
    - no parity

    If the device has a RS485 interface, the default serial bus address is 2.

    @author Alexander Pohl <alex@ahpohl.com>
    */
class ABBAurora
{
private:
  /** @brief Send command
      
      Method for sending a command to the device. The communication protocol uses fixed length transmission messages (8 bytes + 2 bytes for checksum). The structure of the answer also has fixed length (6 bytes + 2 bytes for checksum).

      @param cmd The command described in [ref SendCommandEnum]
      @param b2, b3, b4, b5, b6, b7 Remaining bytes of the transmission message
      */  
  bool Send(SendCommandEnum cmd, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7);
  /// Serial object which handles the communication with the device
  ABBAuroraSerial *Serial;
  /// Address of the serial device
  unsigned char Address;
  /// Array to hold the answer from the device
  uint8_t *ReceiveData;
  /// String which holds the possible error message
  std::string ErrorMessage;
  /// Fixed send buffer size (8 bytes)
  static const int SEND_BUFFER_SIZE;
  /// Fixed receive buffer size (10 bytes)
  static const int RECEIVE_BUFFER_SIZE;

public:
  /** @brief Default class constructor
      
      Initialises the class object with the default bus address
      */
  ABBAurora(void);
  /** @brief Overloaded class constructor
      
      Initialises the class object with a custom bus address

      @param addr RS485 device address, range 2-63
      */
  ABBAurora(const unsigned char &addr);
  /** @brief Default class destructor
      
      Closes the serial port and destroys the class object.
      */
  ~ABBAurora(void);
  /** @brief Setup serial device communication
      
      Opens the host serial device and sets the communication parameters.

      Supported baud rates:
      - 19200 baud (default)
      - 9600 baud
      - 4800 baud
      - 2400 baud

      @param device Serial device, i.e. /dev/ttyUSB0
      @param baudrate Optional baud rate
      */
  bool Setup(const std::string &device, const speed_t baudrate = B19200);
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
  
  /** @name Inverter commands
      */
  ///@{
  
  /** @brief State request

      Ask the state of the system modules
      */
  bool ReadState(void);
  /** @brief Data structure for state request command 
      */
  struct State
  {
    /// Transmission state
    unsigned char TransmissionState;
    /// Global state
    unsigned char GlobalState;
    /// Inverter state
    unsigned char InverterState;
    /// Channel 1 state
    unsigned char Channel1State;
    /// Channel 2 state
    unsigned char Channel2State;
    /// Alarm state
    unsigned char AlarmState;
  } State;
  /** @brief P/N reading

      Reads the system part number (Aurora inverters). No information on transmission and global state is returned.
      */
  bool ReadSystemPN(void);
  /// String for the read system P/N command
  std::string SystemPN;
  /** @brief Version reading

      Reads the version (Indoor/Outdoor, Europe/America, available only for FW version 1.0.9 and following)
      */
  bool ReadVersion(void);
  /** @brief Data structure for version reading command
      */
  struct Version
  {
    /// Transmission state
    unsigned char TransmissionState;
    /// Global state
    unsigned char GlobalState;
    /// Version part 1
    std::string Par1;
    /// Version part 2
    std::string Par2;
    /// Version part 3
    std::string Par3;
    /// Version part 4
    std::string Par4;
  } Version;
  /** @brief Measure request to the DSP

      Sends a request to measure voltage, current, power etc.

      @param type Requested value, described in [Ref DspValueEnum]
      @param global Parameter described in [Ref DspGlobalEnum]
      */ 
  bool ReadDspValue(const DspValueEnum &type, const DspGlobalEnum &global);
  /** @brief Data structure for measure request to dsp command
      */
  struct Dsp
  {
    /// Transmission state
    unsigned char TransmissionState;
    /// Global state
    unsigned char GlobalState;
    /// DSP value
    float Value;
  } Dsp;
  /** @brief Serial number reading

      Reads the system serial number (Aurora inverters). No information on transmission and global state is returned.
      */
  bool ReadSystemSerialNumber(void);
  /// String for the read serial number command
  std::string SystemSerialNumber;
  /** @brief Manufacturing date reading

      Reads the Manufacturing week and year (Aurora inverters)
      */
  bool ReadManufacturingWeekYear(void);
  /** @brief Data structure for the read manufacturing date command
      */
  struct ManufacturingWeekYear
  {
    /// Transmission state
    unsigned char TransmissionState;
    /// Global state
    unsigned char GlobalState;
    /// Manufacturing week
    std::string Week;
    /// Manufacturing year
    std::string Year;
  } ManufacturingWeekYear;
  /** @brief Time/Date reading

      Reads the time and date from the inverter, with one second accuracy
      */
  bool ReadTimeDate(void);
  /** @brief Data structure for time/date reading command
      */
  struct TimeDate
  {
    /// Transmission state
    unsigned char TransmissionState;
    /// Global state
    unsigned char GlobalState;
    /// Seconds since 1st January 2000
    unsigned long Seconds;
    /// Unix epoch
    time_t Epoch;
    /// Human readable time and date
    std::string TimeDate;
  } TimeDate;
  /** @brief Firmware release reading

      Reads the firmware release. For Aurora grid-tied inverters you will read always the MCU firmware version (the field var is not interpreted)
      */
  bool ReadFirmwareRelease(void);
  /** @brief Data structure for the read firmware release command
      */
  struct FirmwareRelease
  {
    /// Transmission state
    unsigned char TransmissionState;
    /// Global state
    unsigned char GlobalState;
    /// Firmware release
    std::string Release;
  } FirmwareRelease;
  /** @brief Cumulated energy readings

      Reads the cumumlated energy (Aurora grid-tied inverters only)

      @param period Energy period described in [ref CumulatedEnergyEnum]
      */
  bool ReadCumulatedEnergy(const CumulatedEnergyEnum &period);
  /** @brief Data structure for the read cumulated energy command
      */
  struct CumulatedEnergy
  {
    /// Transmission state
    unsigned char TransmissionState;
    /// Global state
    unsigned char GlobalState;
    /// Cumulated energy in Wh
    unsigned long Energy;
  } CumulatedEnergy; 
  /** @brief Last four alarms

      The command returns the codes of the last four alarms in form of a FIFO queue from the first to the last one. When this command is used the queue is emptied. Alarm codes are described in [ref to State request]
      */
  bool ReadLastFourAlarms(void);
  /** @brief Data structure for last four alarms command
      */
  struct LastFourAlarms
  {
    /// Transmission state
    unsigned char TransmissionState;
    /// Global state
    unsigned char GlobalState;
    /// Alarm 1
    unsigned char Alarms1;
    /// Alarm 2
    unsigned char Alarms2;
    /// Alarm 3
    unsigned char Alarms3;
    /// Alarm 4
    unsigned char Alarms4;
  } LastFourAlarms;
};

#endif
