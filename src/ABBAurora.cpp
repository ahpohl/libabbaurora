#include <iostream>
#include <cstring>
#include <sstream>
#include <ctime>
#include "ABBAurora.h"
#include "ABBAuroraStrings.h"

const int ABBAurora::SendBufferSize = 10;
const int ABBAurora::ReceiveBufferSize = 8;
const time_t ABBAurora::InverterEpoch = 946684800;

ABBAurora::ABBAurora(void) : Address(2)
{
}

ABBAurora::ABBAurora(const unsigned char &addr) : Address(addr)
{
}

ABBAurora::~ABBAurora(void)
{
  if (ReceiveData) { delete[] ReceiveData; }
  if (Serial) { delete Serial; }
}

bool ABBAurora::Setup(const std::string &device, const speed_t baudrate)
{
  ReceiveData = new uint8_t[ABBAurora::ReceiveBufferSize] ();
  Serial = new ABBAuroraSerial();
  if (!Serial->Begin(device, baudrate))
  {
    ErrorMessage = Serial->GetErrorMessage();
    return false;
  }
  return true;
}

void ABBAurora::SetAddress(const unsigned char &addr)
{
  Address = addr;
}

unsigned char ABBAurora::GetAddress(void) const
{
  return Address;
}

std::string ABBAurora::GetErrorMessage(void) const
{
  return ErrorMessage;
}

bool ABBAurora::Send(SendCommandEnum cmd, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7)
{
  uint8_t SendData[ABBAurora::SendBufferSize] = {0};

  SendData[0] = Address;
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

  memset(ReceiveData, '\0', ABBAurora::ReceiveBufferSize);

  if (Serial->WriteBytes(SendData, ABBAurora::SendBufferSize) < 0)
  {
    ErrorMessage = std::string("Write bytes failed: ") + Serial->GetErrorMessage();
    Serial->Flush();
    return false;
  }
  if (Serial->ReadBytes(ReceiveData, ABBAurora::ReceiveBufferSize) < 0) 
  {
    ErrorMessage = std::string("Read bytes failed: ") + Serial->GetErrorMessage();
    Serial->Flush();
    return false;
  }
  if (!(Serial->Word(ReceiveData[7], ReceiveData[6]) == Serial->Crc16(ReceiveData, 0, 6)))
  {
    ErrorMessage = "Received serial package with CRC mismatch";
    Serial->Flush();
    return false;
  }
  if ((cmd != SendCommandEnum::PN_READING) && (cmd != SendCommandEnum::SERIAL_NUMBER_READING) && ReceiveData[0])
  {
    ErrorMessage = std::string("Transmission error: ") + ABBAuroraStrings::TransmissionState(ReceiveData[0]) + " (" + std::to_string(ReceiveData[0]) + ")";
    return false;
  }
  return true;
}

long int ABBAurora::GetGmtOffset(void)
{
  time_t now = time(NULL);
  struct tm tm;
  gmtime_r(&now, &tm);
  tm.tm_isdst = -1;
  time_t gmt = mktime(&tm);

  return difftime(now, gmt);
}

// Inverter commands

bool ABBAurora::ReadState(ABBAurora::State &state)
{
  if (!Send(SendCommandEnum::STATE_REQUEST, 0, 0, 0, 0, 0, 0))
  {
    return false;
  }
  state.GlobalState = ABBAuroraStrings::GlobalState(ReceiveData[1]);
  state.InverterState = ABBAuroraStrings::InverterState(ReceiveData[2]);
  state.Channel1State = ABBAuroraStrings::DcDcState(ReceiveData[3]);
  state.Channel2State = ABBAuroraStrings::DcDcState(ReceiveData[4]);
  state.AlarmState = ABBAuroraStrings::AlarmState(ReceiveData[5]);
  return true;
}

bool ABBAurora::ReadPartNumber(std::string &pn)
{
  if (!Send(SendCommandEnum::PN_READING, 0, 0, 0, 0, 0, 0))
  {
    return false;
  }
  std::ostringstream oss;
  for (int c = 0; c < 6; c++)
  {
    oss << ReceiveData[c];
  }
  pn = oss.str();
  return true;
}

bool ABBAurora::ReadVersion(ABBAurora::Version &version)
{
  if (!Send(SendCommandEnum::VERSION_READING, 0, 0, 0, 0, 0, 0))
  {
    return false;
  }
  version.GlobalState = ABBAuroraStrings::GlobalState(ReceiveData[1]);
  version.Par1 = ABBAuroraStrings::VersionPart1(ReceiveData[2]);
  version.Par2 = ABBAuroraStrings::VersionPart2(ReceiveData[3]);
  version.Par3 = ABBAuroraStrings::VersionPart3(ReceiveData[4]);
  version.Par4 = ABBAuroraStrings::VersionPart4(ReceiveData[5]);

  return true;
}

bool ABBAurora::ReadDspValue(float &value, const DspValueEnum &type, const DspGlobalEnum &global)
{
  if (!Send(SendCommandEnum::MEASURE_REQUEST_DSP, static_cast<uint8_t>(type), static_cast<uint8_t>(global), 0, 0, 0, 0))
  {
    return false;
  }
  if (ReceiveData[1] != 6) // global state "Run"
  {
    ErrorMessage = std::string("Warning DSP value not trusted: Inverter state \"") + ABBAuroraStrings::GlobalState(ReceiveData[1]) + "\" (" + std::to_string(ReceiveData[1]) + ")";
    return false;
  }
  uint8_t b[] = {ReceiveData[5], ReceiveData[4], ReceiveData[3], ReceiveData[2]};
  memcpy(&value, &b, sizeof(b));
  return true;
}

bool ABBAurora::ReadSerialNumber(std::string &sn)
{
  if (!Send(SendCommandEnum::SERIAL_NUMBER_READING, 0, 0, 0, 0, 0, 0))
  {
    return false;
  }
  std::ostringstream oss;
  for (int c = 0; c < 6; c++) {
    oss << ReceiveData[c];
  }
  sn = oss.str();

  return true;
}

bool ABBAurora::ReadManufacturingDate(ABBAurora::ManufacturingDate &date)
{
  if (!Send(SendCommandEnum::MANUFACTURING_DATE, 0, 0, 0, 0, 0, 0))
  {
    return false;
  }
  date.GlobalState = ABBAuroraStrings::GlobalState(ReceiveData[1]);
  std::ostringstream oss;
  oss << ReceiveData[2] << ReceiveData[3];
  date.Week = oss.str();
  oss.str("");
  oss << ReceiveData[4] << ReceiveData[5];
  date.Year = oss.str();

  return true;
}

bool ABBAurora::ReadTimeDate(ABBAurora::TimeDate &date)
{
  if (!Send(SendCommandEnum::TIME_DATE_READING, 0, 0, 0, 0, 0, 0))
  {
    return false;
  }  
  date.GlobalState = ABBAuroraStrings::GlobalState(ReceiveData[1]);
  uint8_t b[] = {ReceiveData[5], ReceiveData[4], ReceiveData[3], ReceiveData[2]};
  date.InverterTime = 0;
  memcpy(&date.InverterTime, &b, sizeof(b));
  date.EpochTime = date.InverterTime + InverterEpoch - GetGmtOffset();
  struct tm tm;
  localtime_r(&date.EpochTime, &tm);
  char buffer[80];
  strftime(buffer, 80, "%d-%b-%Y %H:%M:%S", &tm);
  date.TimeDate.assign(buffer);
  return true;
}

bool ABBAurora::ReadFirmwareRelease(ABBAurora::FirmwareRelease &firmware)
{
  if (!Send(SendCommandEnum::FIRMWARE_RELEASE_READING, 0, 0, 0, 0, 0, 0))
  {
    return false;
  }
  firmware.GlobalState = ABBAuroraStrings::GlobalState(ReceiveData[1]);
  std::ostringstream oss;
  for (int c = 2; c < 6; c++)
  {
    oss << ReceiveData[c];
    if (c < 5)
    {
      oss << ".";
    }
  }
  firmware.Release = oss.str();
  return true;
}

bool ABBAurora::ReadCumulatedEnergy(float &cum_energy, const CumulatedEnergyEnum &period)
{
  if (!Send(SendCommandEnum::CUMULATED_ENERGY_READINGS, static_cast<uint8_t>(period), 0, 0, 0, 0, 0))
  {
    return false;
  }
  if (ReceiveData[1] != 6) // global state "Run"
  {
    ErrorMessage = std::string("Warning cumulated energy not trusted: Inverter state \"") + ABBAuroraStrings::GlobalState(ReceiveData[1]) + "\" (" + std::to_string(ReceiveData[1]) + ")";
    return false;
  }
  uint8_t b[] = {ReceiveData[5], ReceiveData[4], ReceiveData[3], ReceiveData[2]};
  uint32_t energy_watt_hours = 0;
  memcpy(&energy_watt_hours, &b, sizeof(b));
  cum_energy = static_cast<float>(energy_watt_hours) / 1000.0;
  return true;
}

bool ABBAurora::ReadLastFourAlarms(ABBAurora::LastFourAlarms &alarms)
{
  if (!Send(SendCommandEnum::LAST_FOUR_ALARMS, 0, 0, 0, 0, 0, 0))
  {
    return false;
  } 
  alarms.GlobalState = ABBAuroraStrings::GlobalState(ReceiveData[1]);
  alarms.Alarm1 = ABBAuroraStrings::AlarmState(ReceiveData[2]);
  alarms.Alarm2 = ABBAuroraStrings::AlarmState(ReceiveData[3]);
  alarms.Alarm3 = ABBAuroraStrings::AlarmState(ReceiveData[4]);
  alarms.Alarm4 = ABBAuroraStrings::AlarmState(ReceiveData[5]);
  return true;
}
