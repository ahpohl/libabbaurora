/*
 *  Simple example that shows the basic usage of the ABBAurora class.
 */
#include <iostream>
#include <string>
#include <iomanip> 
#include <ABBAurora.h>

int main(int argc, char *argv[])
{
  int const INVERTER_ADDRESS = 2;
  std::string device = "/dev/ttyUSB0";
  
  ABBAurora *inverter = new ABBAurora(INVERTER_ADDRESS);
  inverter = new ABBAurora(INVERTER_ADDRESS);
  inverter->Setup(device);


  /*
  if (inverter->ReadVersion())
  {
    Serial.print("Inverter Name: ");
    Serial.println(inverter->Version.Par1);
  }
  else
  {
    Serial.print("Inverter could not be reached");
    delay(500);
    return;
  }

  if (inverter->ReadDSPValue(POWER_IN_1, MODULE_MESSUREMENT))
  {
    Serial.print("Pin1 : ");
    Serial.print(inverter->DSP.Value);
    Serial.println(" W");
  }

  if (inverter->ReadCumulatedEnergy(CURRENT_DAY))
  {
    Serial.print("Energy: ");
    Serial.print(inverter->CumulatedEnergy.Energy);
    Serial.println(" Wh");
  }
  */

  delete inverter;

  return EXIT_SUCCESS;
}
