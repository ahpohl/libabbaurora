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
  
  uint8_t *SendData;
  uint8_t *ReceiveData;
  ABBAuroraSerial *Serial;

  void ClearBuffer(uint8_t *buffer, size_t len);

  uint16_t Crc16(uint8_t *data, int offset, int count);
  uint16_t Word(uint8_t msb, uint8_t lsb);
  uint8_t LowByte(uint16_t t_word);
  uint8_t HighByte(uint16_t t_word);

  bool Send(uint8_t address, SendCommandEnum cmd, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7);

  union {
    unsigned char asBytes[4];
    float asFloat;
  } Flo;

  union {
    unsigned char asBytes[4];
    unsigned long asUlong;
  } Ulo;

public:
  ABBAurora(unsigned char addr);
  ~ABBAurora(void);  

  bool SendStatus;
  bool ReceiveStatus;
  unsigned char Address;

  void Setup(std::string &device);

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

  bool ReadDspValue(DspValueEnum type, DspGlobalEnum global);

  typedef struct
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    unsigned long Seconds;
    bool ReadState;
  } DataTimeDate;

  DataTimeDate TimeDate;

  bool ReadTimeDate();

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

  bool ReadJunctionBoxState(unsigned char nj);

  bool ReadJunctionBoxVal(unsigned char nj, unsigned char par);

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

  bool ReadCumulatedEnergy(CumulatedEnergyEnum par);

  bool WriteBaudRateSetting(unsigned char baudcode);

  // Central
  bool ReadFlagsSwitchCentral(void);

  bool ReadCumulatedEnergyCentral(unsigned char var, unsigned char ndays_h, unsigned char ndays_l, unsigned char global);

  bool ReadFirmwareReleaseCentral(unsigned char var);

  bool ReadBaudRateSettingCentral(unsigned char baudcode, unsigned char serialline);

  bool ReadSystemInfoCentral(unsigned char var);

  bool ReadJunctionBoxMonitoringCentral(unsigned char cf, unsigned char rn, unsigned char njt, unsigned char jal, unsigned char jah);

  bool ReadSystemPNCentral(void);

  bool ReadSystemSerialNumberCentral(void);
};

#endif
