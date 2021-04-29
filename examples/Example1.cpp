/*
 *  Simple example that shows the basic usage of the ABBAurora class.
 */
#include <iostream>
#include <string>
#include <iomanip> 
#include "ABBAurora.h"

int main(int argc, char *argv[])
{
  const int inverter_address = 2;
  const std::string device = "/dev/ttyUSB0";
  
  ABBAurora *inverter = new ABBAurora(inverter_address, BaudCodeEnum::BAUD_B19200);
  inverter->Setup(device);

  if (inverter->ReadVersion())
  {
    std::cout << "Inverter Name: " << inverter->Version.Par1 << std::endl;
  }
  else
  {
    throw std::runtime_error(std::string("Inverter could not be reached"));
  }

  if (inverter->ReadDspValue(DspValueEnum::POWER_IN_1, DspGlobalEnum::MODULE_MEASUREMENT))
  {
    std::cout << "Pin1: " << inverter->Dsp.Value << " W" << std::endl;
  }

  if (inverter->ReadCumulatedEnergy(CumulatedEnergyEnum::CURRENT_DAY))
  {
    std::cout << "Energy today: " << inverter->CumulatedEnergy.Energy << " Wh" << std::endl;
  }

  if (inverter)
  {
    delete inverter;
  }

  return EXIT_SUCCESS;
}
