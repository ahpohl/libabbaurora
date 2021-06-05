#include <string>
#include "ABBAuroraStrings.h"

std::string ABBAuroraStrings::TransmissionState(unsigned char id)
{
  switch (id)
  {
  case 0:
    return "Everything is OK.";
  case 51:
    return "Command is not implemented";
  case 52:
    return "Variable does not exist";
  case 53:
    return "Variable value is out of range";
  case 54:
    return "EEprom not accessible";
  case 55:
    return "Not Toggled Service Mode";
  case 56:
    return "Can not send the command to internal micro";
  case 57:
    return "Command not Executed";
  case 58:
    return "The variable is not available, retry";
  default:
    return "Unknown";
  }
}

std::string ABBAuroraStrings::GlobalState(unsigned char id)
{
  switch (id)
  {
  case 0:
    return "Sending Parameters";
  case 1:
    return "Wait Sun / Grid";
  case 2:
    return "Checking Grid";
  case 3:
    return "Measuring Riso";
  case 4:
    return "DcDc Start";
  case 5:
    return "Inverter Start";
  case 6:
    return "Run";
  case 7:
    return "Recovery";
  case 8:
    return "Pause";
  case 9:
    return "Ground Fault";
  case 10:
    return "OTH Fault";
  case 11:
    return "Address Setting";
  case 12:
    return "Self Test";
  case 13:
    return "Self Test Fail";
  case 14:
    return "Sensor Test + Meas.Riso";
  case 15:
    return "Leak Fault";
  case 16:
    return "Waiting for manual reset";
  case 17:
    return "Internal Error E026";
  case 18:
    return "Internal Error E027";
  case 19:
    return "Internal Error E028";
  case 20:
    return "Internal Error E029";
  case 21:
    return "Internal Error E030";
  case 22:
    return "Sending Wind Table";
  case 23:
    return "Failed Sending table";
  case 24:
    return "UTH Fault";
  case 25:
    return "Remote OFF";
  case 26:
    return "Interlock Fail";
  case 27:
    return "Executing Autotest";
  case 30:
    return "Waiting Sun";
  case 31:
    return "Temperature Fault";
  case 32:
    return "Fan Staucked";
  case 33:
    return "Int.Com.Fault";
  case 34:
    return "Slave Insertion";
  case 35:
    return "DC Switch Open";
  case 36:
    return "TRAS Switch Open";
  case 37:
    return "MASTER Exclusion";
  case 38:
    return "Auto Exclusion";
  case 98:
    return "Erasing Internal EEprom";
  case 99:
    return "Erasing External EEprom";
  case 100:
    return "Counting EEprom";
  case 101:
    return "Freeze";
  default:
    return "Unknown";
  }
}

std::string ABBAuroraStrings::DcDcState(unsigned char id)
{
  switch (id)
  {
  case 0:
    return "DcDc OFF";
  case 1:
    return "Ramp Start";
  case 2:
    return "MPPT";
  case 3:
    return "Not Used";
  case 4:
    return "Input OC";
  case 5:
    return "Input UV";
  case 6:
    return "Input OV";
  case 7:
    return "Input Low";
  case 8:
    return "No Parameters";
  case 9:
    return "Bulk OV";
  case 10:
    return "Communication Error";
  case 11:
    return "Ramp Fail";
  case 12:
    return "Internal Error";
  case 13:
    return "Input mode Error";
  case 14:
    return "Ground Fault";
  case 15:
    return "Inverter Fail";
  case 16:
    return "DcDc IGBT Sat";
  case 17:
    return "DcDc ILEAK Fail";
  case 18:
    return "DcDc Grid Fail";
  case 19:
    return "DcDc Comm.Error";
  default:
    return "Unknown";
  }
}

std::string ABBAuroraStrings::InverterState(unsigned char id)
{
  switch (id)
  {
  case 0:
    return "Stand By";
  case 1:
    return "Checking Grid";
  case 2:
    return "Run";
  case 3:
    return "Bulk OV";
  case 4:
    return "Out OC";
  case 5:
    return "IGBT Sat";
  case 6:
    return "Bulk UV";
  case 7:
    return "Degauss Error";
  case 8:
    return "No Parameters";
  case 9:
    return "Bulk Low";
  case 10:
    return "Grid OV";
  case 11:
    return "Communication Error";
  case 12:
    return "Degaussing";
  case 13:
    return "Starting";
  case 14:
    return "Bulk Cap Fail";
  case 15:
    return "Leak Fail";
  case 16:
    return "DcDc Fail";
  case 17:
    return "Ileak Sensor Fail";
  case 18:
    return "SelfTest: relay inverter";
  case 19:
    return "SelfTest : wait for sensor test";
  case 20:
    return "SelfTest : test relay DcDc + sensor";
  case 21:
    return "SelfTest : relay inverter fail";
  case 22:
    return "SelfTest timeout fail";
  case 23:
    return "SelfTest : relay DcDc fail";
  case 24:
    return "Self Test 1";
  case 25:
    return "Waiting self test start";
  case 26:
    return "Dc Injection";
  case 27:
    return "Self Test 2";
  case 28:
    return "Self Test 3";
  case 29:
    return "Self Test 4";
  case 30:
    return "Internal Error";
  case 31:
    return "Internal Error";
  case 40:
    return "Forbidden State";
  case 41:
    return "Input UC";
  case 42:
    return "Zero Power";
  case 43:
    return "Grid Not Present";
  case 44:
    return "Waiting Start";
  case 45:
    return "MPPT";
  case 46:
    return "Grid Fail";
  case 47:
    return "Input OC";
  default:
    return "Unknown";
  }
}

std::string ABBAuroraStrings::AlarmState(unsigned char id)
{
  switch (id)
  {
  case 0:
    return "No Alarm";
  case 1:
    return "Sun Low";
  case 2:
    return "Input OC";
  case 3:
    return "Input UV";
  case 4:
    return "Input OV";
  case 5:
    return "Sun Low";
  case 6:
    return "No Parameters";
  case 7:
    return "Bulk OV";
  case 8:
    return "Comm.Error";
  case 9:
    return "Output OC";
  case 10:
    return "IGBT Sat";
  case 11:
    return "Bulk UV";
  case 12:
    return "Internal error";
  case 13:
    return "Grid Fail";
  case 14:
    return "Bulk Low";
  case 15:
    return "Ramp Fail";
  case 16:
    return "Dc / Dc Fail";
  case 17:
    return "Wrong Mode";
  case 18:
    return "Ground Fault";
  case 19:
    return "Over Temp.";
  case 20:
    return "Bulk Cap Fail";
  case 21:
    return "Inverter Fail";
  case 22:
    return "Start Timeout";
  case 23:
    return "Ground Fault";
  case 24:
    return "Degauss error";
  case 25:
    return "Ileak sens.fail";
  case 26:
    return "DcDc Fail";
  case 27:
    return "Self Test Error 1";
  case 28:
    return "Self Test Error 2";
  case 29:
    return "Self Test Error 3";
  case 30:
    return "Self Test Error 4";
  case 31:
    return "DC inj error";
  case 32:
    return "Grid OV";
  case 33:
    return "Grid UV";
  case 34:
    return "Grid OF";
  case 35:
    return "Grid UF";
  case 36:
    return "Z grid Hi";
  case 37:
    return "Internal error";
  case 38:
    return "Riso Low";
  case 39:
    return "Vref Error";
  case 40:
    return "Error Meas V";
  case 41:
    return "Error Meas F";
  case 42:
    return "Error Meas Z";
  case 43:
    return "Error Meas Ileak";
  case 44:
    return "Error Read V";
  case 45:
    return "Error Read I";
  case 46:
    return "Table fail";
  case 47:
    return "Fan Fail";
  case 48:
    return "UTH";
  case 49:
    return "Interlock fail";
  case 50:
    return "Remote Off";
  case 51:
    return "Vout Avg errror";
  case 52:
    return "Battery low";
  case 53:
    return "Clk fail";
  case 54:
    return "Input UC";
  case 55:
    return "Zero Power";
  case 56:
    return "Fan Stucked";
  case 57:
    return "DC Switch Open";
  case 58:
    return "Tras Switch Open";
  case 59:
    return "AC Switch Open";
  case 60:
    return "Bulk UV";
  case 61:
    return "Autoexclusion";
  case 62:
    return "Grid df / dt";
  case 63:
    return "Den switch Open";
  case 64:
    return "Jbox fail";
  default:
    return "Unknown";
  }
}

std::string ABBAuroraStrings::VersionPart1(char id)
{
  switch (id)
  {
  case 'i':
    return "Aurora 2 kW indoor";
  case 'o':
    return "Aurora 2 kW outdoor";
  case 'I':
    return "Aurora 3.6 kW indoor";
  case 'O':
    return "Aurora 3.0 - 3.6 kW outdoor";
  case '5':
    return "Aurora 5.0 kW outdoor";
  case '6':
    return "Aurora 6 kW outdoor";
  case 'P':
    return "3 - phase interface (3G74)";
  case 'C':
    return "Aurora 50kW module";
  case '4':
    return "Aurora 4.2kW new";
  case '3':
    return "Aurora 3.6kW new";
  case '2':
    return "Aurora 3.3kW new";
  case '1':
    return "Aurora 3.0kW new";
  case 'D':
    return "Aurora 12.0kW";
  case 'X':
    return "Aurora 10.0kW";
  default:
    return "Unknown";
  }
}

std::string ABBAuroraStrings::VersionPart2(char id)
{
  switch (id)
  {
  case 'A':
    return "UL1741";
  case 'E':
    return "VDE0126";
  case 'S':
    return "DR 1663 / 2000";
  case 'I':
    return "ENEL DK 5950";
  case 'U':
    return "UK G83";
  case 'K':
    return "AS 4777";
  default:
    return "Unknown";
  }
}

std::string ABBAuroraStrings::VersionPart3(char id)
{
  switch (id)
  {
  case 'N':
    return "Transformerless Version";
  case 'T':
    return "Transformer Version";
  default:
    return "Unknown";
  }
}

std::string ABBAuroraStrings::VersionPart4(char id)
{
  switch (id)
  {
  case 'W':
    return "Wind version";
  case 'N':
    return "PV version";
  default:
    return "Unknown";
  }
}
