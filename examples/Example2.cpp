/** @example Example2.cpp

    This example shows how meta data such as the firmware version or manufacturing date is obtained from the Aurora device.
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
    std::cout << "Inverter setup failed: " << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }

  if (!inverter->ReadTimeDate())
  {
    std::cout << "Read date and time failed: " << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Date: " << inverter->TimeDate.TimeDate << std::endl;

  return EXIT_SUCCESS;
}
