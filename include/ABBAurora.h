#ifndef ABBAurora_h
#define ABBAurora_h
#include <string>
#include "ABBAuroraEnums.h"
#include "ABBAuroraSerial.h"

class ABBAurora
{
private:
  int MaxAttempt = 1;
  ABBAuroraSerial *serial;

  void clearBuffer(uint8_t *buffer, size_t len);

  uint16_t Crc16(uint8_t *data, int offset, int count);
  uint16_t Word(uint8_t msb, uint8_t lsb);
  uint8_t lowByte(uint16_t t_word);
  uint8_t highByte(uint16_t t_word);

  bool Send(uint8_t address, uint8_t cmd, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7);

  union {
    unsigned char asBytes[4];
    float asFloat;
  } foo;

  union {
    unsigned char asBytes[4];
    unsigned long asUlong;
  } ulo;

public:
  bool SendStatus = false;
  bool ReceiveStatus = false;
  unsigned char Address = 0;
  uint8_t ReceiveData[8];

  static void Setup(std::string &device);

  ABBAurora(unsigned char address);
  ~ABBAurora(void);

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

  bool ReadState();

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

  bool ReadVersion();

  typedef struct
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    float Value;
    bool ReadState;
  } DataDSP;

  DataDSP DSP;

  bool ReadDSPValue(DSP_VALUE_TYPE type, DSP_GLOBAL global);

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

  bool ReadLastFourAlarms();

  bool ReadJunctionBoxState(unsigned char nj);

  bool ReadJunctionBoxVal(unsigned char nj, unsigned char par);

  // Inverters
  typedef struct
  {
    std::string PN;
    bool ReadState;
  } DataSystemPN;

  DataSystemPN SystemPN;

  bool ReadSystemPN();

  typedef struct
  {
    std::string SerialNumber;
    bool ReadState;
  } DataSystemSerialNumber;

  DataSystemSerialNumber SystemSerialNumber;

  bool ReadSystemSerialNumber();

  typedef struct
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    std::string Week;
    std::string Year;
    bool ReadState;
  } DataManufacturingWeekYear;

  DataManufacturingWeekYear ManufacturingWeekYear;

  bool ReadManufacturingWeekYear();

  typedef struct
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    std::string Release;
    bool ReadState;
  } DataFirmwareRelease;

  DataFirmwareRelease FirmwareRelease;

  bool ReadFirmwareRelease();

  typedef struct
  {
    unsigned char TransmissionState;
    unsigned char GlobalState;
    unsigned long Energy;
    bool ReadState;
  } DataCumulatedEnergy;

  DataCumulatedEnergy CumulatedEnergy;

  bool ReadCumulatedEnergy(CUMULATED_ENERGY_TYPE par);

  bool WriteBaudRateSetting(unsigned char baudcode);

  // Central
  bool ReadFlagsSwitchCentral();

  bool ReadCumulatedEnergyCentral(unsigned char var, unsigned char ndays_h, unsigned char ndays_l, unsigned char global);

  bool ReadFirmwareReleaseCentral(unsigned char var);

  bool ReadBaudRateSettingCentral(unsigned char baudcode, unsigned char serialline);

  bool ReadSystemInfoCentral(unsigned char var);

  bool ReadJunctionBoxMonitoringCentral(unsigned char cf, unsigned char rn, unsigned char njt, unsigned char jal, unsigned char jah);

  bool ReadSystemPNCentral();

  bool ReadSystemSerialNumberCentral();
};

#endif
