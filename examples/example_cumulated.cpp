/** @example example_cumulated.cpp

    This example shows cumulated energy readings.
    ```
    Current day: 27.82 kWh
    Current week: 111.54 kWh
    Current month: 225.99 kWh
    Current year: 1912.22 kWh
    Lifetime total: 47435.21 kWh
    Since reset: 47435.11 kWh
    ```
    The output is equivalent to "aurora -a 2 -e /dev/ttyUSB0"
    */
#include "ABBAurora.h"
#include "ABBAuroraEnums.h"
#include <iostream>
#include <memory>

int main(int argc, char *argv[]) {
  const std::string device = "/dev/ttyUSB0";

  std::unique_ptr<ABBAurora> inverter(new ABBAurora());

  if (!inverter->Setup(device)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }

  std::ios::fmtflags old_settings = std::cout.flags();
  std::cout.precision(2);
  std::cout.setf(std::ios::fixed, std::ios::floatfield);

  float current_day;
  if (!inverter->ReadCumulatedEnergy(current_day,
                                     CumulatedEnergyEnum::CURRENT_DAY)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Current day: " << current_day << " kWh" << std::endl;

  float current_week;
  if (!inverter->ReadCumulatedEnergy(current_week,
                                     CumulatedEnergyEnum::CURRENT_WEEK)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Current week: " << current_week << " kWh" << std::endl;

  float current_month;
  if (!inverter->ReadCumulatedEnergy(current_month,
                                     CumulatedEnergyEnum::CURRENT_MONTH)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Current month: " << current_month << " kWh" << std::endl;

  float current_year;
  if (!inverter->ReadCumulatedEnergy(current_year,
                                     CumulatedEnergyEnum::CURRENT_YEAR)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Current year: " << current_year << " kWh" << std::endl;

  float lifetime_total;
  if (!inverter->ReadCumulatedEnergy(lifetime_total,
                                     CumulatedEnergyEnum::LIFETIME_TOTAL)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Lifetime total: " << lifetime_total << " kWh" << std::endl;

  float since_reset;
  if (!inverter->ReadCumulatedEnergy(since_reset,
                                     CumulatedEnergyEnum::SINCE_RESET)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "Since reset: " << since_reset << " kWh" << std::endl;

  std::cout.flags(old_settings);
  return EXIT_SUCCESS;
}
