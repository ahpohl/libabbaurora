/** @example example_daemon.cpp

    This example shows how to create a simple daemon.
    */
#include "ABBAurora.h"
#include <chrono>
#include <csignal>
#include <iostream>
#include <memory>
#include <thread>

volatile sig_atomic_t shutdown = false;

void sig_handler(int) { shutdown = true; }

int main(int argc, char *argv[]) {
  struct sigaction action;
  action.sa_handler = sig_handler;
  sigemptyset(&action.sa_mask);
  action.sa_flags = SA_RESTART;
  sigaction(SIGINT, &action, NULL);
  sigaction(SIGTERM, &action, NULL);

  const std::string device = "/dev/ttyUSB0";
  std::unique_ptr<ABBAurora> inverter(new ABBAurora());
  if (!inverter->Setup(device)) {
    std::cout << inverter->GetErrorMessage() << std::endl;
    return EXIT_FAILURE;
  }

  std::ios::fmtflags old_settings = std::cout.flags();
  std::cout.precision(2);
  std::cout.setf(std::ios::fixed, std::ios::floatfield);

  while (shutdown == false) {
    float grid_power;
    if (!inverter->ReadDspValue(grid_power, DspValueEnum::GRID_POWER)) {
      std::cout << inverter->GetErrorMessage() << std::endl;
      return EXIT_FAILURE;
    }
    std::cout << "Grid Power Reading: " << grid_power << " W" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  std::cout.flags(old_settings);
  return EXIT_SUCCESS;
}
