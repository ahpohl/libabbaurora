/** @example example_inverter.cpp

    This example shows basic information about the Aurora device.
    ```
    Current inverter date/time: 06-Jun-2021 11:15:11
    Part number: -3G79-
    Serial number: 126014
    Firmware: C.0.2.2
    Manufacturing Date: Year 10 Week 20
    Version: -- Aurora 4.2kW new -- VDE0126 -- Transformerless Version -- PV
   version --
    ```
    The output is equivalent to "aurora -a 2 -f -g -n -p -v -t /dev/ttyUSB0"
    */
#include "ABBAurora.h"
#include <iostream>
#include <memory>

int main(int argc, char *argv[]) {
  const std::string device = "/dev/ttyUSB0";
  std::unique_ptr<ABBAurora> inverter(new ABBAurora());
  if (!inverter->Setup(device)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }

  ABBAurora::TimeDate date;
  if (!inverter->ReadTimeDate(date)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Current inverter date/time: " << date.TimeDate << std::endl;

  std::string part_number;
  if (!inverter->ReadPartNumber(part_number)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Part number: " << part_number << std::endl;

  std::string serial_number;
  if (!inverter->ReadSerialNumber(serial_number)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Serial number: " << serial_number << std::endl;

  ABBAurora::FirmwareRelease firmware;
  if (!inverter->ReadFirmwareRelease(firmware)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Firmware: " << firmware.Release << std::endl;

  ABBAurora::ManufacturingDate mfg_date;
  if (!inverter->ReadManufacturingDate(mfg_date)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Manufacturing Date: Year " << mfg_date.Year << " Week "
            << mfg_date.Week << std::endl;

  ABBAurora::Version version;
  if (!inverter->ReadVersion(version)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Version: -- " << version.Par1 << " -- " << version.Par2
            << " -- " << version.Par3 << " -- " << version.Par4 << " -- "
            << std::endl;

  return EXIT_SUCCESS;
}
