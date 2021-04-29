#ifndef ABBAurora_h
#define ABBAurora_h
#include <string>
#include "ABBAuroraEnums.h"
#include "ABBAuroraSerial.h"

class ABBAurora
{
private:
  static const int SEND_BUFFER_SIZE;
  static const int RECEIVE_BUFFER_SIZE;
  static const SendCommandEnum SEND_COMMAND;
  
  uint8_t *ReceiveData;
  ABBAuroraSerial *Serial;
  speed_t GetBaudRate(const BaudCodeEnum &baudcode) const;

  bool Send(SendCommandEnum cmd, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7);

  union {
    unsigned char asBytes[4];
    float asFloat;
  } Flo;

  union {
    unsigned char asBytes[4];
    unsigned long asUlong;
  } Ulo;

public:
  ABBAurora(const unsigned char &addr);
  ABBAurora(const unsigned char &addr, const BaudCodeEnum &baud);
  ~ABBAurora(void);  

  bool ReceiveStatus;
  unsigned char Address;

  void SetAddress(const unsigned char &addr);
  unsigned char GetAddress(void);

  void Setup(const std::string &device);

  typedef struct
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    unsigned char InverterState;
    unsigned char Channel1State;
    unsigned char Channel2State;
    unsigned char AlarmState;
    bool ReadState;
  } DataState;

  DataState State;

  bool ReadState(void);

  typedef struct
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    std::string Par1;
    std::string Par2;
    std::string Par3;
    std::string Par4;
    bool ReadState;
  } DataVersion;

  DataVersion Version;

  bool ReadVersion(void);

  typedef struct
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    float Value;
    bool ReadState;
  } DataDsp;

  DataDsp Dsp;

  bool ReadDspValue(const DspValueEnum &type, const DspGlobalEnum &global);

  typedef struct
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    unsigned long Seconds;
    bool ReadState;
  } DataTimeDate;

  DataTimeDate TimeDate;

  bool ReadTimeDate(void);

  typedef struct
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    unsigned char Alarms1;
    unsigned char Alarms2;
    unsigned char Alarms3;
    unsigned char Alarms4;
    bool ReadState;
  } DataLastFourAlarms;

  DataLastFourAlarms LastFourAlarms;

  bool ReadLastFourAlarms(void);

  bool ReadJunctionBoxState(const unsigned char &nj);

  bool ReadJunctionBoxVal(const unsigned char &nj, const unsigned char &par);

  // Inverters
  typedef struct
  {
    std::string PN;
    bool ReadState;
  } DataSystemPN;

  DataSystemPN SystemPN;

  bool ReadSystemPN(void);

  typedef struct
  {
    std::string SerialNumber;
    bool ReadState;
  } DataSystemSerialNumber;

  DataSystemSerialNumber SystemSerialNumber;

  bool ReadSystemSerialNumber(void);

  typedef struct
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    std::string Week;
    std::string Year;
    bool ReadState;
  } DataManufacturingWeekYear;

  DataManufacturingWeekYear ManufacturingWeekYear;

  bool ReadManufacturingWeekYear(void);

  typedef struct
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    std::string Release;
    bool ReadState;
  } DataFirmwareRelease;

  DataFirmwareRelease FirmwareRelease;

  bool ReadFirmwareRelease(void);

  typedef struct
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    unsigned long Energy;
    bool ReadState;
  } DataCumulatedEnergy;

  DataCumulatedEnergy CumulatedEnergy;

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
