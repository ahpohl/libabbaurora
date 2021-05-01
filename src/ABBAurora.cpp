#include <cstring>
#include <sstream>
#include "ABBAurora.h"
#include "ABBAuroraStrings.h"
#include "ABBAuroraSerial.h"
#include "ABBAuroraEnums.h"

const int ABBAurora::SEND_BUFFER_SIZE = 10;
const int ABBAurora::RECEIVE_BUFFER_SIZE = 8; 

ABBAurora::ABBAurora(void) : 
  Address(2), BaudCode(BaudCodeEnum::BAUD_B19200)
{
}

ABBAurora::ABBAurora(const unsigned char &addr) : 
  Address(addr), BaudCode(BaudCodeEnum::BAUD_B19200)
{
}

ABBAurora::ABBAurora(const unsigned char &addr, const BaudCodeEnum &baud) : 
  Address(addr), BaudCode(baud)
{
}

ABBAurora::~ABBAurora(void)
{
  if (Serial) { delete Serial; }
  if (ReceiveData) { delete[] ReceiveData; }
}

speed_t ABBAurora::GetBaudRate(const BaudCodeEnum &baudcode) const
{
  speed_t baudrate;
  
  switch (baudcode)
  {
    case BaudCodeEnum::BAUD_B19200:
      baudrate = B19200;
      break;
    case BaudCodeEnum::BAUD_B9600:
      baudrate = B9600;
      break;
    case BaudCodeEnum::BAUD_B4800:
      baudrate = B4800;
      break;
    case BaudCodeEnum::BAUD_B2400:
      baudrate = B2400;
      break;
    default:
      baudrate = B0;
      break;
  }
  return baudrate;
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

bool ABBAurora::Setup(const std::string &device)
{
  ReceiveData = new uint8_t[ABBAurora::RECEIVE_BUFFER_SIZE] ();
  Serial = new ABBAuroraSerial();
  speed_t baudrate = GetBaudRate(BaudCode);
  if (!Serial->Begin(device, baudrate))
  {
    ErrorMessage = Serial->GetErrorMessage();
    return false;
  }

  return true;
}

bool ABBAurora::Send(SendCommandEnum cmd, uint8_t b2, uint8_t b3, uint8_t b4, uint8_t b5, uint8_t b6, uint8_t b7)
{
  uint8_t SendData[ABBAurora::SEND_BUFFER_SIZE] = {0};;
  
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

  memset(ReceiveData, '\0', ABBAurora::RECEIVE_BUFFER_SIZE);

  if (Serial->WriteBytes(SendData, ABBAurora::SEND_BUFFER_SIZE) < 0)
  {
    ErrorMessage = std::string("Write bytes failed: ") + Serial->GetErrorMessage();
    return false;
  }
  if (Serial->ReadBytes(ReceiveData, ABBAurora::RECEIVE_BUFFER_SIZE) < 0) 
  {
    ErrorMessage = std::string("Read bytes failed: ") + Serial->GetErrorMessage();
    return false;
  }
  if (!(Serial->Word(ReceiveData[7], ReceiveData[6]) == Serial->Crc16(ReceiveData, 0, 6)))
  {
    ErrorMessage = "Received serial package with CRC mismatch";
    return false;
  }
  return true;
}

bool ABBAurora::ReadDspValue(const DspValueEnum &type, const DspGlobalEnum &global)
{
  if (!Send(SendCommandEnum::MEASURE_REQUEST_DSP, static_cast<uint8_t>(type), static_cast<uint8_t>(global), 0, 0, 0, 0))
  {
    return false;
  }
  Dsp.TransmissionState = ReceiveData[0];
  if (Dsp.TransmissionState)
  {
    ErrorMessage = ABBAuroraStrings::TransmissionState(Dsp.TransmissionState);
    return false;
  }
  Dsp.GlobalState = ReceiveData[1];
  Flo.asBytes[0] = ReceiveData[5];
  Flo.asBytes[1] = ReceiveData[4];
  Flo.asBytes[2] = ReceiveData[3];
  Flo.asBytes[3] = ReceiveData[2];
  Dsp.Value = Flo.asFloat;

  return true;
}

bool ABBAurora::ReadTimeDate(void)
{
  if (!Send(SendCommandEnum::TIME_DATE_READING, 0, 0, 0, 0, 0, 0))
  {
    return false;
  }  
  TimeDate.TransmissionState = ReceiveData[0];
  if (TimeDate.TransmissionState)
  {
    ErrorMessage = ABBAuroraStrings::TransmissionState(TimeDate.TransmissionState);
    return false;
  }
  TimeDate.GlobalState = ReceiveData[1];
  TimeDate.Seconds = (static_cast<uint32_t>(ReceiveData[2]) << 24) + (static_cast<uint32_t>(ReceiveData[3]) << 16) + (static_cast<uint32_t>(ReceiveData[4]) << 8) + static_cast<uint32_t>(ReceiveData[5]);
  
  return true;
}

bool ABBAurora::ReadLastFourAlarms(void)
{
  if (!Send(SendCommandEnum::LAST_FOUR_ALARMS, 0, 0, 0, 0, 0, 0))
  {
    return false;
  } 
  LastFourAlarms.TransmissionState = ReceiveData[0];
  if (LastFourAlarms.TransmissionState)
  {
    ErrorMessage = ABBAuroraStrings::TransmissionState(LastFourAlarms.TransmissionState);
    return false;
  }
  LastFourAlarms.GlobalState = ReceiveData[1];
  LastFourAlarms.Alarms1 = ReceiveData[2];
  LastFourAlarms.Alarms2 = ReceiveData[3];
  LastFourAlarms.Alarms3 = ReceiveData[4];
  LastFourAlarms.Alarms4 = ReceiveData[5];

  return true;
}

bool ABBAurora::ReadSystemPN(void)
{
  if (!Send(SendCommandEnum::PN_READING, 0, 0, 0, 0, 0, 0))
  {
    return false;
  }
  std::ostringstream convert;
  for (int c = 0; c < 6; c++)
  {
    convert << ReceiveData[c];
  }
  SystemPN = convert.str();

  return true;
}

bool ABBAurora::ReadSystemSerialNumber(void)
{
  if (!Send(SendCommandEnum::SERIAL_NUMBER_READING, 0, 0, 0, 0, 0, 0))
  {
    return false;
  }
  std::ostringstream convert;
  for (int c = 0; c < 6; c++) {
    convert << ReceiveData[c];
  }
  SystemSerialNumber = convert.str();

  return true;
}

bool ABBAurora::ReadManufacturingWeekYear(void)
{
  if (!Send(SendCommandEnum::MANUFACTURING_DATE, 0, 0, 0, 0, 0, 0))
  {
    return false;
  }

  ManufacturingWeekYear.TransmissionState = ReceiveData[0];
  if (ManufacturingWeekYear.TransmissionState)
  {
    ErrorMessage = ABBAuroraStrings::TransmissionState(ManufacturingWeekYear.TransmissionState);
    return false;
  }
  ManufacturingWeekYear.GlobalState = ReceiveData[1];
  ManufacturingWeekYear.Week = std::to_string(ReceiveData[2]) + std::to_string(ReceiveData[3]);
  ManufacturingWeekYear.Year = std::to_string(ReceiveData[4]) + std::to_string(ReceiveData[5]);

  return true;
}

bool ABBAurora::ReadFirmwareRelease(void)
{
  if (!Send(SendCommandEnum::FIRMWARE_RELEASE_READING, 0, 0, 0, 0, 0, 0))
  {
    return false;
  }
  FirmwareRelease.TransmissionState = ReceiveData[0];
  if (FirmwareRelease.TransmissionState)
  {
    ErrorMessage = ABBAuroraStrings::TransmissionState(FirmwareRelease.TransmissionState);
    return false;
  }
  FirmwareRelease.GlobalState = ReceiveData[1];
  std::ostringstream convert;
  for (int c = 2; c < 6; c++)
  {
    convert << ReceiveData[c];
  }
  FirmwareRelease.Release = convert.str();

  return true;
}

bool ABBAurora::ReadCumulatedEnergy(const CumulatedEnergyEnum &period)
{
  if (!Send(SendCommandEnum::CUMULATED_ENERGY_READING, static_cast<uint8_t>(period), 0, 0, 0, 0, 0))
  {
    return false;
  }
  CumulatedEnergy.TransmissionState = ReceiveData[0];
  if (CumulatedEnergy.TransmissionState)
  {
    ErrorMessage = ABBAuroraStrings::TransmissionState(CumulatedEnergy.TransmissionState);
    return false;
  }
  CumulatedEnergy.GlobalState = ReceiveData[1];
  Ulo.asBytes[0] = ReceiveData[5];
  Ulo.asBytes[1] = ReceiveData[4];
  Ulo.asBytes[2] = ReceiveData[3];
  Ulo.asBytes[3] = ReceiveData[2];
  CumulatedEnergy.Energy = Ulo.asUlong;

  return true;
}

bool ABBAurora::WriteBaudRateSetting(const BaudCodeEnum &baudcode, const unsigned char &serialline)
{
  return Send(SendCommandEnum::BAUD_RATE_SETTING, static_cast<unsigned char>(baudcode), serialline, 0, 0, 0, 0);
}

bool ABBAurora::ReadState(void)
{
  if (!Send(SendCommandEnum::STATE_REQUEST, 0, 0, 0, 0, 0, 0))
  {
    return false;
  }
  State.TransmissionState = ReceiveData[0];
  {
    ErrorMessage = ABBAuroraStrings::TransmissionState(State.TransmissionState);
    return false;
  }
  State.GlobalState = ReceiveData[1];
  State.InverterState = ReceiveData[2];
  State.Channel1State = ReceiveData[3];
  State.Channel2State = ReceiveData[4];
  State.AlarmState = ReceiveData[5];

  return true;
}

bool ABBAurora::ReadVersion(void)
{
  if (!Send(SendCommandEnum::VERSION_READING, 0, 0, 0, 0, 0, 0))
  {
    return false;
  }
  Version.TransmissionState = ReceiveData[0];
  if (Version.TransmissionState)
  {
    ErrorMessage = ABBAuroraStrings::TransmissionState(Version.TransmissionState);
    return false;
  }
  Version.GlobalState = ReceiveData[1];
  Version.Par1 = ABBAuroraStrings::VersionPart1(ReceiveData[2]);
  Version.Par2 = ABBAuroraStrings::VersionPart2(ReceiveData[3]);
  Version.Par3 = ABBAuroraStrings::VersionPart3(ReceiveData[4]);
  Version.Par4 = ABBAuroraStrings::VersionPart4(ReceiveData[5]);

  return true;
}
