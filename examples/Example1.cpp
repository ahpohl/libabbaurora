/** @example Example1.cpp

    Simple example that shows the basic usage of the ABBAurora class.
    */
#include <iostream>
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

  if (!inverter->ReadVersion())
  {
    std::cout << "Read version failed: " << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Inverter Name: " << inverter->Version.Par1 << std::endl;

  if (!inverter->ReadDspValue(DspValueEnum::POWER_IN_1, DspGlobalEnum::MODULE_MEASUREMENT))
  {
    std::cout << "Read DSP value failed: " << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Pin1: " << inverter->Dsp.Value << " W" << std::endl;

  if (!inverter->ReadCumulatedEnergy(CumulatedEnergyEnum::CURRENT_DAY))
  {
    std::cout << "Read cumulated energy failed: " << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Energy today: " << inverter->CumulatedEnergy.Energy << " Wh" << std::endl;

  if (inverter)
  {
    delete inverter;
  }

  return EXIT_SUCCESS;
}
