/** @example example_alarms.cpp

    This example reads the last four alarms.
    ```
    Global state: Run
    Alarm 1: No Alarm
    Alarm 2: No Alarm
    Alarm 3: No Alarm
    Alarm 4: No Alarm
    ```
    The output is equivalent to "aurora -a 2 -A /dev/ttyUSB0"
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

  ABBAurora::LastFourAlarms alarms;
  if (!inverter->ReadLastFourAlarms(alarms))
  {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Global state: " << alarms.GlobalState << std::endl;
  std::cout << "Alarm 1: " << alarms.Alarm1 << std::endl;
  std::cout << "Alarm 2: " << alarms.Alarm2 << std::endl;
  std::cout << "Alarm 3: " << alarms.Alarm3 << std::endl;
  std::cout << "Alarm 4: " << alarms.Alarm4 << std::endl;

  return EXIT_SUCCESS;
}
