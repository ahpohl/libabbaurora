#include <cstring>
#include <sstream>
#include "ABBAurora.h"
#include "ABBAuroraStrings.h"
#include "ABBAuroraSerial.h"
#include "ABBAuroraEnums.h"

const int ABBAurora::SEND_BUFFER_SIZE = 10;
const int ABBAurora::RECEIVE_BUFFER_SIZE = 8; 

ABBAurora::ABBAurora(unsigned char addr)
{
  Address = addr;
  SendStatus = false;
  ReceiveStatus = false;
}

ABBAurora::~ABBAurora()
{
  if (Serial) { delete Serial; }
  if (SendData) { delete[] SendData; }
  if (ReceiveData) { delete[] ReceiveData; }
}

void ABBAurora::ClearBuffer(uint8_t *buffer, size_t len) const
{
  memset(buffer, '\0', len);
}

void ABBAurora::Setup(std::string &device)
{
  SendData = new uint8_t[ABBAurora::SEND_BUFFER_SIZE] ();
  ReceiveData = new uint8_t[ABBAurora::RECEIVE_BUFFER_SIZE] ();
  Serial = new ABBAuroraSerial();
  Serial->Begin(device);
}

bool ABBAurora::Send(uint8_t address, SendCommandEnum cmd, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7)
{
  SendStatus = false;
  ReceiveStatus = false;

  SendData[0] = address;
  SendData[1] = static_cast<uint8_t>(cmd);
  SendData[2] = b2;
  SendData[3] = b3;
  SendData[4] = b4;
  SendData[5] = b5;
  SendData[6] = b6;
  SendData[7] = b7;

  uint16_t crc = Serial->Crc16(SendData, 0, 8);
  SendData[8] = Serial->LowByte(crc);
  SendData[9] = Serial->HighByte(crc);

  ClearBuffer(ReceiveData, ABBAurora::RECEIVE_BUFFER_SIZE);

  if (Serial->WriteBytes(SendData, ABBAurora::SEND_BUFFER_SIZE) > 0)
  {
    SendStatus = true;
    if (Serial->ReadBytes(ReceiveData, ABBAurora::RECEIVE_BUFFER_SIZE) > 0)
    {
      if (Serial->Word(ReceiveData[7], ReceiveData[6]) == Serial->Crc16(ReceiveData, 0, 6))
      {
        ReceiveStatus = true;
      }
    }
  }

  return ReceiveStatus;
}

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
bool ABBAurora::ReadDspValue(DspValueEnum type, DspGlobalEnum global)
{
  if (((static_cast<uint8_t>(type) >= 1 && static_cast<uint8_t>(type) <= 9) || (static_cast<uint8_t>(type) >= 21 && static_cast<uint8_t>(type) <= 63)) && static_cast<uint8_t>(global) <= 1)
  {
    Dsp.ReadState = Send(Address, SendCommandEnum::MEASURE_REQUEST_DSP, static_cast<uint8_t>(type), static_cast<uint8_t>(global), 0, 0, 0, 0);

    if (Dsp.ReadState == false)
    {
      ReceiveData[0] = 255;
      ReceiveData[1] = 255;
    }
  }
  else
  {
    Dsp.ReadState = false;
    ClearBuffer(ReceiveData, ABBAurora::RECEIVE_BUFFER_SIZE);
    ReceiveData[0] = 255;
    ReceiveData[1] = 255;
  }

  Dsp.TransmissionState = ReceiveData[0];
  Dsp.GlobalState = ReceiveData[1];

  Flo.asBytes[0] = ReceiveData[5];
  Flo.asBytes[1] = ReceiveData[4];
  Flo.asBytes[2] = ReceiveData[3];
  Flo.asBytes[3] = ReceiveData[2];

  Dsp.Value = Flo.asFloat;

  return Dsp.ReadState;
}

bool ABBAurora::ReadTimeDate()
{
  TimeDate.ReadState = Send(Address, SendCommandEnum::TIME_DATE_READING, 0, 0, 0, 0, 0, 0);

  if (TimeDate.ReadState == false)
  {
    ReceiveData[0] = 255;
    ReceiveData[1] = 255;
  }

  TimeDate.TransmissionState = ReceiveData[0];
  TimeDate.GlobalState = ReceiveData[1];
  TimeDate.Seconds = (static_cast<uint32_t>(ReceiveData[2]) << 24) + (static_cast<uint32_t>(ReceiveData[3]) << 16) + (static_cast<uint32_t>(ReceiveData[4]) << 8) + static_cast<uint32_t>(ReceiveData[5]);
  return TimeDate.ReadState;
}

bool ABBAurora::ReadLastFourAlarms(void)
{
  LastFourAlarms.ReadState = Send(Address, SendCommandEnum::LAST_FOUR_ALARMS, 0, 0, 0, 0, 0, 0);

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
  return Send(Address, SendCommandEnum::JB_STATE_REQUEST, nj, 0, 0, 0, 0, 0);
}

bool ABBAurora::ReadJunctionBoxVal(unsigned char nj, unsigned char par)
{
  return Send(Address, SendCommandEnum::JB_VAL_REQUEST, nj, par, 0, 0, 0, 0);
}


// Inverters

bool ABBAurora::ReadSystemPN(void)
{
  SystemPN.ReadState = Send(Address, SendCommandEnum::PN_READING, 0, 0, 0, 0, 0, 0);

  std::ostringstream convert;
  for (int c = 0; c < 6; c++)
  {
    convert << ReceiveData[c];
  }
  SystemPN.PN = convert.str();

  return SystemPN.ReadState;
}

bool ABBAurora::ReadSystemSerialNumber(void)
{
  SystemSerialNumber.ReadState = Send(Address, SendCommandEnum::SERIAL_NUMBER_READING, 0, 0, 0, 0, 0, 0);

  std::ostringstream convert;
  for (int c = 0; c < 6; c++) {
    convert << ReceiveData[c];
  }
  SystemSerialNumber.SerialNumber = convert.str();

  return SystemSerialNumber.ReadState;
}

bool ABBAurora::ReadManufacturingWeekYear()
{
  ManufacturingWeekYear.ReadState = Send(Address, SendCommandEnum::MANUFACTURING_DATE, 0, 0, 0, 0, 0, 0);

  if (ManufacturingWeekYear.ReadState == false)
  {
    ReceiveData[0] = 255;
    ReceiveData[1] = 255;
  }

  ManufacturingWeekYear.TransmissionState = ReceiveData[0];
  ManufacturingWeekYear.GlobalState = ReceiveData[1];
  ManufacturingWeekYear.Week = std::to_string(ReceiveData[2]) + std::to_string(ReceiveData[3]);
  ManufacturingWeekYear.Year = std::to_string(ReceiveData[4]) + std::to_string(ReceiveData[5]);

  return ManufacturingWeekYear.ReadState;
}

bool ABBAurora::ReadFirmwareRelease(void)
{
  FirmwareRelease.ReadState = Send(Address, SendCommandEnum::FIRMWARE_RELEASE_READING, 0, 0, 0, 0, 0, 0);

  if (FirmwareRelease.ReadState == false)
  {
    ReceiveData[0] = 255;
    ReceiveData[1] = 255;
  }

  FirmwareRelease.TransmissionState = ReceiveData[0];
  FirmwareRelease.GlobalState = ReceiveData[1];

  std::ostringstream convert;
  for (int c = 2; c < 6; c++)
  {
    convert << ReceiveData[c];
  }
  FirmwareRelease.Release = convert.str();

  return FirmwareRelease.ReadState;
}

bool ABBAurora::ReadCumulatedEnergy(CumulatedEnergyEnum par)
{
  if (static_cast<uint8_t>(par) <= 6)
  {
    CumulatedEnergy.ReadState = Send(Address, SendCommandEnum::CUMULATED_ENERGY_READINGS, static_cast<uint8_t>(par), 0, 0, 0, 0, 0);
    if (CumulatedEnergy.ReadState == false)
    {
      ReceiveData[0] = 255;
      ReceiveData[1] = 255;
    }
  }
  else
  {
    CumulatedEnergy.ReadState = false;
    ClearBuffer(ReceiveData, ABBAurora::RECEIVE_BUFFER_SIZE);
    ReceiveData[0] = 255;
    ReceiveData[1] = 255;
  }

  CumulatedEnergy.TransmissionState = ReceiveData[0];
  CumulatedEnergy.GlobalState = ReceiveData[1];
  if (CumulatedEnergy.ReadState == true)
  {
    Ulo.asBytes[0] = ReceiveData[5];
    Ulo.asBytes[1] = ReceiveData[4];
    Ulo.asBytes[2] = ReceiveData[3];
    Ulo.asBytes[3] = ReceiveData[2];

    CumulatedEnergy.Energy = Ulo.asUlong;
  }
  return CumulatedEnergy.ReadState;
}

bool ABBAurora::WriteBaudRateSetting(unsigned char baudcode)
{
  if (baudcode <= 3)
  {
    return Send(Address, SendCommandEnum::BAUD_RATE_SETTING, baudcode, 0, 0, 0, 0, 0);
  }
  else
  {
    ClearBuffer(ReceiveData, ABBAurora::RECEIVE_BUFFER_SIZE);
    return false;
  }
}

// Central
bool ABBAurora::ReadFlagsSwitchCentral(void)
{
  return Send(Address, SendCommandEnum::FLAGS_SWITCH_READING, 0, 0, 0, 0, 0, 0);
}

bool ABBAurora::ReadCumulatedEnergyCentral(unsigned char var, unsigned char ndays_h, unsigned char ndays_l, unsigned char global)
{
  return Send(Address, SendCommandEnum::CUMULATED_ENERGY_CENTRAL, var, ndays_h, ndays_l, global, 0, 0);
}

bool ABBAurora::ReadFirmwareReleaseCentral(unsigned char var)
{
  return Send(Address, SendCommandEnum::FIRMWARE_RELEASE_READING, var, 0, 0, 0, 0, 0);
}

bool ABBAurora::ReadBaudRateSettingCentral(unsigned char baudcode, unsigned char serialline)
{
  return Send(Address, SendCommandEnum::BAUD_RATE_SETTING, baudcode, serialline, 0, 0, 0, 0);
}

bool ABBAurora::ReadSystemInfoCentral(unsigned char var)
{
  return Send(Address, SendCommandEnum::SYSTEM_INFO_CENTRAL, var, 0, 0, 0, 0, 0);
}

bool ABBAurora::ReadJunctionBoxMonitoringCentral(unsigned char cf, unsigned char rn, unsigned char njt, unsigned char jal, unsigned char jah)
{
  return Send(Address, SendCommandEnum::JB_MONITORING_STATUS, cf, rn, njt, jal, jah, 0);
}

bool ABBAurora::ReadSystemPNCentral(void)
{
  return Send(Address, SendCommandEnum::PN_READING_CENTRAL, 0, 0, 0, 0, 0, 0);
}

bool ABBAurora::ReadSystemSerialNumberCentral()
{
  return Send(Address, SendCommandEnum::SERIAL_NUMBER_CENTRAL, 0, 0, 0, 0, 0, 0);
}

bool ABBAurora::ReadState(void)
{
  State.ReadState = Send(Address, SendCommandEnum::STATE_REQUEST, 0, 0, 0, 0, 0, 0);

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

bool ABBAurora::ReadVersion(void)
{
  Version.ReadState = Send(Address, SendCommandEnum::VERSION_READING, 0, 0, 0, 0, 0, 0);

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
