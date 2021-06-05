#ifndef ABBAurora_h
#define ABBAurora_h
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
  static const int SendBufferSize; ///< Fixed send buffer size (8 bytes)
  static const int ReceiveBufferSize; ///< Fixed receive buffer size (10 bytes)
  static const time_t InverterEpoch; ///< Seconds since midnight of January 1, 2000.

private:
  ABBAuroraSerial *Serial; ///< Serial object which handles the communication with the device
  unsigned char Address; ///< Address of the serial device
  uint8_t *ReceiveData; ///< Array to hold the answer from the device
  std::string ErrorMessage; ///< String which holds the possible error message
  /** @brief Send command
      
      Method for sending a command to the device. The communication protocol uses fixed length transmission messages (8 bytes + 2 bytes for checksum). The structure of the answer also has fixed length (6 bytes + 2 bytes for checksum).

      @param cmd The command to send
      @param b2, b3, b4, b5, b6, b7 Remaining bytes of the transmission message
      */
  bool Send(SendCommandEnum cmd, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7);
  /** @brief Get GMT offset

      Method which returns the current offset to GMT [in seconds]
      */
  long int GetGmtOffset(void);

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

      @returns Current RS485 serial device address     
      */
  unsigned char GetAddress(void) const;
  /** @brief Get error message

      @returns Error message
      */
  std::string GetErrorMessage(void) const;
  
  /** @name Inverter commands
      */
  ///@{

  struct State /// Data structure for ReadState()
  {
    std::string GlobalState; ///< Global state
    std::string InverterState; ///< Inverter state
    std::string Channel1State; ///< Channel 1 state
    std::string Channel2State; ///< Channel 2 state
    std::string AlarmState; ///< Alarm state
  };  
  /** @brief Read system state

      Ask the state of the system modules
     
      @param state State of the system
      */
  bool ReadState(State &state);
  /** @brief Read system P/N

      Reads the part number (only Aurora inverters). No information on transmission and global state is returned.
      
      @param pn Inverter part number
      */
  bool ReadPartNumber(std::string &pn);

  struct Version /// Data structure for version reading command
  {
    std::string GlobalState; ///< Global state
    std::string Par1; ///< Version part 1
    std::string Par2; ///< Version part 2
    std::string Par3; ///< Version part 3
    std::string Par4; ///< Version part 4
  };
  /** @brief Read version

      Reads the version (Indoor/Outdoor, Europe/America, available only for FW version 1.0.9 and following)

      @param version Inverter version
      */
  bool ReadVersion(Version &version);

  /** @brief Measure request to the DSP

      Sends a request to measure voltage, current, power etc.

      @param value DSP value
      @param type The value to read
      @param global Optional scope parameter
      */ 
  bool ReadDspValue(float &value, const DspValueEnum &type, const DspGlobalEnum &global = DspGlobalEnum::MODULE_MEASUREMENT);
  /** @brief Read serial number

      Reads the system serial number (Aurora inverters). No information on transmission and global state is returned.
       
      @param sn Serial number
      */
  bool ReadSerialNumber(std::string &sn);

  struct ManufacturingDate /// Data structure for the read manufacturing date command
  {
    std::string GlobalState; ///< Global state
    std::string Week; ///< Manufacturing week
    std::string Year; ///< Manufacturing year
  };
  /** @brief Read manufacturing date

      Reads the Manufacturing week and year (Aurora inverters)

      @param date Manufacturing date
      */
  bool ReadManufacturingDate(ManufacturingDate &date);

  struct TimeDate /// Data structure for time/date reading command
  {
    std::string GlobalState; ///< Global state
    time_t InverterTime; ///< Seconds since 1st January 2000
    time_t EpochTime; ///< Seconds since unix epoch
    std::string TimeDate; ///< Human readable time and date
  };
  /** @brief Read inverter date

      Reads the time and date from the inverter, with one second accuracy
      
      @param date Inverter date
      */
  bool ReadTimeDate(TimeDate &date);

  struct FirmwareRelease /// Data structure for the read firmware release command
  {
    std::string GlobalState; ///< Global state
    std::string Release; ///< Firmware release
  };
  /** @brief Read firmware release

      Reads the firmware release. For Aurora grid-tied inverters you will read always the MCU firmware version (the field var is not interpreted)

      @param firmware Firmware release
      */
  bool ReadFirmwareRelease(FirmwareRelease &firmware);

  /** @brief Read cumulated energy

      Reads the cumumlated energy (Aurora grid-tied inverters only)

      @param cum_energy Cumulated energy 
      @param period Energy period
      */
  bool ReadCumulatedEnergy(float &cum_energy, const CumulatedEnergyEnum &period);

  struct LastFourAlarms /// Data structure for last four alarms command
  {
    std::string GlobalState; ///< Global state
    std::string Alarm1; ///< Alarm 1
    std::string Alarm2; ///< Alarm 2
    std::string Alarm3; ///< Alarm 3
    std::string Alarm4; ///< Alarm 4
  };
  /** @brief Read last four alarms

      The command returns the codes of the last four alarms in form of a FIFO queue from the first to the last one. When this command is used the queue is emptied.

      @param alarms Last four alarms
      */
  bool ReadLastFourAlarms(LastFourAlarms &alarms);
};

#endif
