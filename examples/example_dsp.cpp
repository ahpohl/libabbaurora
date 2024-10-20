/** @example example_dsp.cpp

    This example reads both DSPs to get voltage, current, power and grid data.
    ```
    put 1 Current: 3.67 A
    Input 1 Power: 976.95 W

    Input 2 Voltage: 269.17 V
    Input 2 Current: 3.71 A
    Input 2 Power: 996.11 W

    Grid Voltage Reading: 236.19 V
    Grid Current Reading: 8.14 A
    Grid Power Reading: 1909.63 W
    Frequency Reading: 50.00 Hz

    DC/AC Conversion Efficiency: 96.79 %
    Inverter Temperature: 48.75 °C
    Booster Temperature: 45.75 °C
    ```
    The output is equivalent to "aurora -a 2 -d 0 /dev/ttyUSB0"
    */
#include "ABBAurora.h"
#include <iomanip>
#include <iostream>
#include <memory>

int main(int argc, char *argv[]) {
  const std::string device = "/dev/ttyUSB0";

  std::unique_ptr<ABBAurora> inverter(new ABBAurora());

  if (!inverter->Setup(device)) {
    std::cout << "Inverter setup failed: " << inverter->GetErrorMessage()
              << std::endl;
    return EXIT_FAILURE;
  }

  std::ios::fmtflags old_settings = std::cout.flags();
  std::cout.precision(2);
  std::cout.setf(std::ios::fixed, std::ios::floatfield);

  float voltage_in_1;
  if (!inverter->ReadDspValue(voltage_in_1, DspValueEnum::V_IN_1)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Input 1 Voltage: " << voltage_in_1 << " V" << std::endl;

  float current_in_1;
  if (!inverter->ReadDspValue(current_in_1, DspValueEnum::I_IN_1)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Input 1 Current: " << current_in_1 << " A" << std::endl;

  float power_in_1;
  if (!inverter->ReadDspValue(power_in_1, DspValueEnum::POWER_IN_1)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Input 1 Power: " << power_in_1 << " W" << std::endl;
  std::cout << std::endl;

  float voltage_in_2;
  if (!inverter->ReadDspValue(voltage_in_2, DspValueEnum::V_IN_2)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Input 2 Voltage: " << voltage_in_2 << " V" << std::endl;

  float current_in_2;
  if (!inverter->ReadDspValue(current_in_2, DspValueEnum::I_IN_2)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Input 2 Current: " << current_in_2 << " A" << std::endl;

  float power_in_2;
  if (!inverter->ReadDspValue(power_in_2, DspValueEnum::POWER_IN_2)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Input 2 Power: " << power_in_2 << " W" << std::endl;
  std::cout << std::endl;

  float grid_voltage;
  if (!inverter->ReadDspValue(grid_voltage, DspValueEnum::GRID_VOLTAGE)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Grid Voltage Reading: " << grid_voltage << " V" << std::endl;

  float grid_current;
  if (!inverter->ReadDspValue(grid_current, DspValueEnum::GRID_CURRENT)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Grid Current Reading: " << grid_current << " A" << std::endl;

  float grid_power;
  if (!inverter->ReadDspValue(grid_power, DspValueEnum::GRID_POWER)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Grid Power Reading: " << grid_power << " W" << std::endl;

  float freq;
  if (!inverter->ReadDspValue(freq, DspValueEnum::FREQUENCY)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Frequency Reading: " << freq << " Hz" << std::endl;
  std::cout << std::endl;

  std::cout << "DC/AC Conversion Efficiency: "
            << (grid_power / (power_in_1 + power_in_2) * 100) << " %"
            << std::endl;

  float inverter_temp;
  if (!inverter->ReadDspValue(inverter_temp,
                              DspValueEnum::TEMPERATURE_INVERTER)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Inverter Temperature: " << inverter_temp << " °C" << std::endl;

  float booster_temp;
  if (!inverter->ReadDspValue(booster_temp,
                              DspValueEnum::TEMPERATURE_BOOSTER)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Booster Temperature: " << booster_temp << " °C" << std::endl;
  std::cout << std::endl;

  std::cout.flags(old_settings);
  return EXIT_SUCCESS;
}
