/*
 *  Simple example that shows the basic usage of the ABBAurora class.
 */
#include <iostream>
#include <string>
#include <iomanip> 
#include "ABBAurora.h"

int main(int argc, char *argv[])
{
  int const INVERTER_ADDRESS = 2;
  std::string device = "/dev/ttyUSB0";
  
  ABBAurora *inverter = new ABBAurora(INVERTER_ADDRESS);
  inverter = new ABBAurora(INVERTER_ADDRESS);
  inverter->Setup(device);

  if (inverter->ReadVersion())
  {
    std::cout << "Inverter Name: " << inverter->Version.Par1 << std::endl;
  }
  else
  {
    throw std::runtime_error(std::string("Inverter could not be reached"));
  }

  if (inverter->ReadDSPValue(POWER_IN_1, MODULE_MESSUREMENT))
  {
    std::cout << "Pin1: " << inverter->DSP.Value << " W" << std::endl;
  }

  if (inverter->ReadCumulatedEnergy(CURRENT_DAY))
  {
    std::cout << "Energy today: " << inverter->CumulatedEnergy.Energy << " Wh" << std::endl;
  }

  delete inverter;

  return EXIT_SUCCESS;
}
