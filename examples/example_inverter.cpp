/** @example example_inverter.cpp

    This example shows basic information about the Aurora device.

    It is equivalent to "aurora -a 2 -f -g -n -p -v -t /dev/ttyUSB0"
    */
#include <iostream>
#include <memory>
#include "ABBAurora.h"

int main(int argc, char *argv[])
{
  const std::string device = "/dev/ttyUSB0";
  
  std::unique_ptr<ABBAurora> inverter(new ABBAurora());

  if (!inverter->Setup(device))
  {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  if (!inverter->ReadTimeDate())
  {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Current inverter date/time: " << inverter->TimeDate.TimeDate << std::endl;
  if (!inverter->ReadFirmwareRelease())
  {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Firmware: " << inverter->FirmwareRelease.Release << std::endl;
  if (!inverter->ReadVersion())
  {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Version: -- " 
    << inverter->Version.Par1 << " -- " 
    << inverter->Version.Par2 << " -- " 
    << inverter->Version.Par3 << " -- "
    << inverter->Version.Par4 << " -- " << std::endl;

  return EXIT_SUCCESS;
}
