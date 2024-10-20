/** @example example_state.cpp

    This example shows the inverter state.
    ```
    Transmission State: Everything is OK.
    Global State: Run
    Inverter State: Run
    Channel 1 State: MPPT
    Channel 2 State: MPPT
    Alarm State: No Alarm
    ```
    The output is equivalent to "aurora -a 2 -s /dev/ttyUSB0"
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

  ABBAurora::State state;
  if (!inverter->ReadState(state)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Global State: " << state.GlobalState << std::endl;
  std::cout << "Inverter State: " << state.InverterState << std::endl;
  std::cout << "Channel 1 State: " << state.Channel1State << std::endl;
  std::cout << "Channel 2 State: " << state.Channel2State << std::endl;
  std::cout << "Alarm State: " << state.AlarmState << std::endl;

  return EXIT_SUCCESS;
}
