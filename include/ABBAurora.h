#ifndef ABBAurora_h
#define ABBAurora_h
#include <ABBAuroraEnums.h>

//RS485 control
//#define RS485Transmit HIGH
//#define RS485Receive LOW

class ABBAurora
{
private:
    int MaxAttempt = 1;
    //static unsigned char TXPinControl;
    //static HardwareSerial *serial;

    void clearData(unsigned char *data, unsigned char len);

    int Crc16(unsigned char *data, int offset, int count);

    bool Send(unsigned char address, unsigned char param0, unsigned char param1, unsigned char param2, unsigned char param3, unsigned char param4, unsigned char param5, unsigned char param6);

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
    unsigned char ReceiveData[8];

    static void setup(HardwareSerial &serial, unsigned char RXGpioPin, unsigned char TXGpioPin, unsigned char TXControllPin);

    ABBAurora(unsigned char address);

    void clearReceiveData();

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
        String Par1;
        String Par2;
        String Par3;
        String Par4;
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
        String PN;
        bool ReadState;
    } DataSystemPN;

    DataSystemPN SystemPN;

    bool ReadSystemPN();

    typedef struct
    {
        String SerialNumber;
        bool ReadState;
    } DataSystemSerialNumber;

    DataSystemSerialNumber SystemSerialNumber;

    bool ReadSystemSerialNumber();

    typedef struct
    {
        unsigned char TransmissionState;
        unsigned char GlobalState;
        String Week;
        String Year;
        bool ReadState;
    } DataManufacturingWeekYear;

    DataManufacturingWeekYear ManufacturingWeekYear;

    bool ReadManufacturingWeekYear();

    typedef struct
    {
        unsigned char TransmissionState;
        unsigned char GlobalState;
        String Release;
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
