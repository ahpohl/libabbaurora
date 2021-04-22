#include "ABBAurora.h"
#include "ABBAuroraStrings.h"
#include "ABBAuroraSerial.h"

ABBAurora::ABBAurora(unsigned char address)
{
    Address = address;
    SendStatus = false;
    ReceiveStatus = false;
    clearReceiveData();
}

void ABBAurora::Setup(std::string &device)
{
    ABBAuroraSerial *serial;
    serial->begin(device);
}

/*
void ABBAurora::clearData(uint8_t *data, size_t &len)
{
    for (size_t i = 0; i < len; i++)
    {
        data[i] = 0;
    }
}

uint16_t Crc16(uint8_t *data, int &offset, int &count);
{
    byte BccLo = 0xFF;
    byte BccHi = 0xFF;

    for (int i = offset; i < (offset + count); i++)
    {
        uint8_t New = data[offset + i] ^ BccLo;
        uint8_t Tmp = New << 4;
        New = Tmp ^ New;
        Tmp = New >> 5;
        BccLo = BccHi;
        BccHi = New ^ Tmp;
        Tmp = New << 3;
        BccLo = BccLo ^ Tmp;
        Tmp = New >> 4;
        BccLo = BccLo ^ Tmp;
    }

    return Word(~BccHi, ~BccLo);
}

uint16_t ABBAurora::Word(uint8_t &crc_hi, uint8_t &crc_lo)
{
  return ((crc_hi & 0xFF) << 8) | crc_lo;
}

bool ABBAurora::Send(unsigned char address, unsigned char param0, unsigned char param1, unsigned char param2, unsigned char param3, unsigned char param4, unsigned char param5, unsigned char param6)
{

    SendStatus = false;
    ReceiveStatus = false;

    unsigned char SendData[10];
    SendData[0] = address;
    SendData[1] = param0;
    SendData[2] = param1;
    SendData[3] = param2;
    SendData[4] = param3;
    SendData[5] = param4;
    SendData[6] = param5;
    SendData[7] = param6;

    int crc = Crc16(SendData, 0, 8);
    SendData[8] = lowByte(crc);
    SendData[9] = highByte(crc);

    clearReceiveData();

    for (int i = 0; i < this->MaxAttempt; i++)
    {
        if (serial->write(SendData, sizeof(SendData)) != 0)
        {
            serial->flush();
            SendStatus = true;

            if (serial->readBytes(ReceiveData, sizeof(ReceiveData)) != 0)
            {
                if (Word(ReceiveData[7], ReceiveData[6]) == Crc16(ReceiveData, 0, 6))
                {
                    ReceiveStatus = true;
                    break;
                }
            }
        }
    }
    return ReceiveStatus;
}

void ABBAurora::clearReceiveData()
{
    clearData(ReceiveData, 8);
}
*/

/**
 * Reads a single value of the digital signal procesor.
 * Not all values are supported by all models. 
 * Read values are in following Units:
 * Voltage V
 * Current A 
 * Power W 
 * Temperature °C 
 * 
 **/

/*
bool ABBAurora::ReadDSPValue(DSP_VALUE_TYPE type, DSP_GLOBAL global)
{
    if (((type >= 1 && type <= 9) || (type >= 21 && type <= 63)) && (global >= 0 && global <= 1))
    {
        DSP.ReadState = Send(this->Address, 59, type, global, 0, 0, 0, 0);

        if (DSP.ReadState == false)
        {
            ReceiveData[0] = 255;
            ReceiveData[1] = 255;
        }
    }
    else
    {
        DSP.ReadState = false;
        clearReceiveData();
        ReceiveData[0] = 255;
        ReceiveData[1] = 255;
    }

    DSP.TransmissionState = ReceiveData[0];
    DSP.GlobalState = ReceiveData[1];

    foo.asBytes[0] = ReceiveData[5];
    foo.asBytes[1] = ReceiveData[4];
    foo.asBytes[2] = ReceiveData[3];
    foo.asBytes[3] = ReceiveData[2];

    DSP.Value = foo.asFloat;

    return DSP.ReadState;
}

bool ABBAurora::ReadTimeDate()
{
    TimeDate.ReadState = Send(this->Address, 70, 0, 0, 0, 0, 0, 0);

    if (TimeDate.ReadState == false)
    {
        ReceiveData[0] = 255;
        ReceiveData[1] = 255;
    }

    TimeDate.TransmissionState = ReceiveData[0];
    TimeDate.GlobalState = ReceiveData[1];
    TimeDate.Seconds = ((unsigned long)ReceiveData[2] << 24) + ((unsigned long)ReceiveData[3] << 16) + ((unsigned long)ReceiveData[4] << 8) + (unsigned long)ReceiveData[5];
    return TimeDate.ReadState;
}

bool ABBAurora::ReadLastFourAlarms()
{
    LastFourAlarms.ReadState = Send(this->Address, 86, 0, 0, 0, 0, 0, 0);

    if (LastFourAlarms.ReadState == false)
    {
        ReceiveData[0] = 255;
        ReceiveData[1] = 255;
        ReceiveData[2] = 255;
        ReceiveData[3] = 255;
        ReceiveData[4] = 255;
        ReceiveData[5] = 255;
    }

    LastFourAlarms.TransmissionState = ReceiveData[0];
    LastFourAlarms.GlobalState = ReceiveData[1];
    LastFourAlarms.Alarms1 = ReceiveData[2];
    LastFourAlarms.Alarms2 = ReceiveData[3];
    LastFourAlarms.Alarms3 = ReceiveData[4];
    LastFourAlarms.Alarms4 = ReceiveData[5];

    return LastFourAlarms.ReadState;
}

bool ABBAurora::ReadJunctionBoxState(unsigned char nj)
{
    return Send(this->Address, 200, nj, 0, 0, 0, 0, 0);
}

bool ABBAurora::ReadJunctionBoxVal(unsigned char nj, unsigned char par)
{
    return Send(this->Address, 201, nj, par, 0, 0, 0, 0);
}

// Inverters

bool ABBAurora::ReadSystemPN()
{
    SystemPN.ReadState = Send(this->Address, 52, 0, 0, 0, 0, 0, 0);

    SystemPN.PN = std::string(std::string((char)ReceiveData[0]) + std::string((char)ReceiveData[1]) + std::string((char)ReceiveData[2]) + std::string((char)ReceiveData[3]) + std::string((char)ReceiveData[4]) + std::string((char)ReceiveData[5]));

    return SystemPN.ReadState;
}

bool ABBAurora::ReadSystemSerialNumber()
{
    SystemSerialNumber.ReadState = Send(this->Address, 63, 0, 0, 0, 0, 0, 0);

    SystemSerialNumber.SerialNumber = std::string(std::string((char)ReceiveData[0]) + std::string((char)ReceiveData[1]) + std::string((char)ReceiveData[2]) + std::string((char)ReceiveData[3]) + std::string((char)ReceiveData[4]) + std::string((char)ReceiveData[5]));

    return SystemSerialNumber.ReadState;
}

bool ABBAurora::ReadManufacturingWeekYear()
{
    ManufacturingWeekYear.ReadState = Send(this->Address, 65, 0, 0, 0, 0, 0, 0);

    if (ManufacturingWeekYear.ReadState == false)
    {
        ReceiveData[0] = 255;
        ReceiveData[1] = 255;
    }

    ManufacturingWeekYear.TransmissionState = ReceiveData[0];
    ManufacturingWeekYear.GlobalState = ReceiveData[1];
    ManufacturingWeekYear.Week = std::string(std::string((char)ReceiveData[2]) + std::string((char)ReceiveData[3]));
    ManufacturingWeekYear.Year = std::string(std::string((char)ReceiveData[4]) + std::string((char)ReceiveData[5]));

    return ManufacturingWeekYear.ReadState;
}

bool ABBAurora::ReadFirmwareRelease()
{
    FirmwareRelease.ReadState = Send(this->Address, 72, 0, 0, 0, 0, 0, 0);

    if (FirmwareRelease.ReadState == false)
    {
        ReceiveData[0] = 255;
        ReceiveData[1] = 255;
    }

    FirmwareRelease.TransmissionState = ReceiveData[0];
    FirmwareRelease.GlobalState = ReceiveData[1];
    FirmwareRelease.Release = std::string(std::string((char)ReceiveData[2]) + "." + std::string((char)ReceiveData[3]) + "." + std::string((char)ReceiveData[4]) + "." + std::string((char)ReceiveData[5]));

    return FirmwareRelease.ReadState;
}

bool ABBAurora::ReadCumulatedEnergy(CUMULATED_ENERGY_TYPE par)
{
    if ((int)par >= 0 && (int)par <= 6)
    {
        CumulatedEnergy.ReadState = Send(this->Address, 78, par, 0, 0, 0, 0, 0);

        if (CumulatedEnergy.ReadState == false)
        {
            ReceiveData[0] = 255;
            ReceiveData[1] = 255;
        }
    }
    else
    {
        CumulatedEnergy.ReadState = false;
        clearReceiveData();
        ReceiveData[0] = 255;
        ReceiveData[1] = 255;
    }

    CumulatedEnergy.TransmissionState = ReceiveData[0];
    CumulatedEnergy.GlobalState = ReceiveData[1];
    if (CumulatedEnergy.ReadState == true)
    {
        ulo.asBytes[0] = ReceiveData[5];
        ulo.asBytes[1] = ReceiveData[4];
        ulo.asBytes[2] = ReceiveData[3];
        ulo.asBytes[3] = ReceiveData[2];

        CumulatedEnergy.Energy = ulo.asUlong;
    }
    return CumulatedEnergy.ReadState;
}

bool ABBAurora::WriteBaudRateSetting(unsigned char baudcode)
{
    if ((int)baudcode >= 0 && (int)baudcode <= 3)
    {
        return Send(this->Address, 85, baudcode, 0, 0, 0, 0, 0);
    }
    else
    {
        clearReceiveData();
        return false;
    }
}

// Central
bool ABBAurora::ReadFlagsSwitchCentral()
{
    return Send(this->Address, 67, 0, 0, 0, 0, 0, 0);
}

bool ABBAurora::ReadCumulatedEnergyCentral(unsigned char var, unsigned char ndays_h, unsigned char ndays_l, unsigned char global)
{
    return Send(this->Address, 68, var, ndays_h, ndays_l, global, 0, 0);
}

bool ABBAurora::ReadFirmwareReleaseCentral(unsigned char var)
{
    return Send(this->Address, 72, var, 0, 0, 0, 0, 0);
}

bool ABBAurora::ReadBaudRateSettingCentral(unsigned char baudcode, unsigned char serialline)
{
    return Send(this->Address, 85, baudcode, serialline, 0, 0, 0, 0);
}

bool ABBAurora::ReadSystemInfoCentral(unsigned char var)
{
    return Send(this->Address, 101, var, 0, 0, 0, 0, 0);
}

bool ABBAurora::ReadJunctionBoxMonitoringCentral(unsigned char cf, unsigned char rn, unsigned char njt, unsigned char jal, unsigned char jah)
{
    return Send(this->Address, 103, cf, rn, njt, jal, jah, 0);
}

bool ABBAurora::ReadSystemPNCentral()
{
    return Send(this->Address, 105, 0, 0, 0, 0, 0, 0);
}

bool ABBAurora::ReadSystemSerialNumberCentral()
{
    return Send(this->Address, 107, 0, 0, 0, 0, 0, 0);
}

bool ABBAurora::ReadState()
{
    State.ReadState = Send(this->Address, 50, 0, 0, 0, 0, 0, 0);

    if (State.ReadState == false)
    {
        ReceiveData[0] = 255;
        ReceiveData[1] = 255;
        ReceiveData[2] = 255;
        ReceiveData[3] = 255;
        ReceiveData[4] = 255;
        ReceiveData[5] = 255;
    }

    State.TransmissionState = ReceiveData[0];
    State.GlobalState = ReceiveData[1];
    State.InverterState = ReceiveData[2];
    State.Channel1State = ReceiveData[3];
    State.Channel2State = ReceiveData[4];
    State.AlarmState = ReceiveData[5];

    return State.ReadState;
}

bool ABBAurora::ReadVersion()
{
    Version.ReadState = Send(this->Address, 58, 0, 0, 0, 0, 0, 0);

    if (Version.ReadState == false)
    {
        ReceiveData[0] = 255;
        ReceiveData[1] = 255;
    }

    Version.TransmissionState = ReceiveData[0];
    Version.GlobalState = ReceiveData[1];

    Version.Par1 = ABBAuroraStrings::VersionPart1(ReceiveData[2]);
    Version.Par2 = ABBAuroraStrings::VersionPart2(ReceiveData[3]);
    Version.Par3 = ABBAuroraStrings::VersionPart3(ReceiveData[4]);
    Version.Par4 = ABBAuroraStrings::VersionPart4(ReceiveData[5]);

    return Version.ReadState;
}

*/
