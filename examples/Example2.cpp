/*
 *  Simple example that shows the basic usage of the ABBAurora class.
 */
#include <iostream>
#include <string>
#include <iomanip> 
#include "ABBAurora.h"

int main(int argc, char *argv[])
{
  const std::string device = "/dev/ttyUSB0";
  
  ABBAurora *inverter = new ABBAurora();
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

  if (inverter)
  {
    delete inverter;
  }

  return EXIT_SUCCESS;
}
