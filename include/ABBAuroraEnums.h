#ifndef ABBAuroraEnums_h
#define ABBAuroraEnums_h

/** @file */ 

/** @brief Measure request to the DSP enum

    The * in the enum below list the variable to which apply the global requirement.

    The value is expressed in the following measurements units:
    - Voltages [V]
    - Currents [A]
    - Powers [W]
    - Temperatures [°C]
    */
enum class DspValueEnum : unsigned char
{
  /// Grid voltage*
  GRID_VOLTAGE = 1,
  /// Grid current*
  GRID_CURRENT = 2,
  /// Grid power*
  GRID_POWER = 3,
  /// Frequency
  FREQUENCY = 4,
  /// Vbulk
  V_BULK = 5,
  /// Ileak (DC/DC)
  I_LEAK_DC_DC = 6,
  /// Ileak (inverter)
  I_LEAK_INVERTER = 7,
  /// Pin1*
  POWER_IN_1 = 8,
  /// Pin2
  POWER_IN_2 = 9,
  /// Inverter temperature
  TEMPERATURE_INVERTER = 21,
  /// Booster temperature
  TEMPERATURE_BOOSTER = 22,
  /// Input 1 voltage
  V_IN_1 = 23,
  /// Input 1 current*
  I_IN_1 = 25,
  /// Input 2 voltage
  V_IN_2 = 26,
  /// Input 2 current
  I_IN_2 = 27,
  /// Grid voltage (DC/DC)
  DC_DC_GRID_VOLTAGE = 28,
  /// Grid frequency (DC/DC)
  DC_DC_GRID_FREQUENCY = 29,
  /// Isolation resistance (Riso)
  ISOLATION_RESISTANCE = 30,
  /// Vbulk (DC/DC)
  DC_DC_V_BULK = 31,
  /// Average grid voltage (VgridAvg)
  AVERAGE_GRID_VOLTAGE = 32,
  /// VbulkMid
  V_BULK_MID = 33,
  /// Power peak
  POWER_PEAK = 34,
  /// Power peak today
  POWER_PEAK_TODAY = 35,
  /// Grid voltage neutral
  GRID_VOLTAGE_NEUTRAL = 36,
  /// Wind generator frequency
  WIND_GENERATOR_FREQENCY = 37,
  /// Grid voltage neutral phase
  GRID_VOLTAGE_NEUTRAL_PHASE = 38,
  /// Grid current phase r
  GRID_CURRENT_PHASE_R = 39,
  /// Grid current phase s
  GRID_CURRENT_PHASE_S = 40,
  /// Grid current phase t
  GRID_CURRENT_PHASE_T = 41,
  /// Frequency phase r
  FREQUENCY_PHASE_R = 42,
  /// Frequency phase s
  FREQUENCY_PHASE_S = 43,
  /// Frequency phase t
  FREQUENCY_PHASE_T = 44,
  /// Vbulk +
  V_BULK_POSITIVE = 45,
  /// Vbulk -
  V_BULK_NEGATIVE = 46,
  /// Supervisor temperature
  TEMPERATURE_SUPERVISOR = 47,
  /// Alim. temperature
  TEMPERATURE_ALIM = 48,
  /// Heat sink temperature
  TEMPERATURE_HEAT_SINK = 49,
  /// Temperature 1
  TEMPERATURE_1 = 50,
  /// Temperature 2
  TEMPERATURE_2 = 51,
  /// Temperature 3
  TEMPERATURE_3 = 52,
  /// Fan speed 1
  FAN_SPEED_1 = 53,
  /// Fan speed 2
  FAN_SPEED_2 = 54,
  /// Fan speed 3
  FAN_SPEED_3 = 55,
  /// Fan speed 4
  FAN_SPEED_4 = 56,
  /// Fan speed 5
  FAN_SPEED_5 = 57,
  /// Power saturation limit (Der.)
  POWER_SATURATION_LIMIT = 58,
  /// V panel micro
  V_PANEL_MICRO = 60,
  /// Grid voltage phase r
  GRID_VOLTAGE_PHASE_R = 61,
  /// Grid voltage phase s
  GRID_VOLTAGE_PHASE_S = 62,
  /// Grid voltage phase t
  GRID_VOLTAGE_PHASE_T = 63
};

/** @brief Dsp global enum

    Global: if 1 requires the global measurements (only for a master)
            if 0 requires the module measurements (master and slave)
    */
enum class DspGlobalEnum : unsigned char
{
  MODULE_MEASUREMENT = 0, GLOBAL_MEASUREMENT = 1
};

/// Cumulated energy enum
enum class CumulatedEnergyEnum : unsigned char
{
  /// Daily energy
  CURRENT_DAY = 0,
  /// Weekly energy
  CURRENT_WEEK = 1,
  /// Month energy (energy from the first day of the current calendar month)
  CURRENT_MONTH = 3,
  /// Year energy (energy from the first day of current calendar year)
  CURRENT_YEAR = 4,
  /// Total energy (total lifetime)
  LIFETIME_TOTAL = 5,
  /// Partial energy (cumulated since reset)
  SINCE_RESET = 6
};

/// Send command enum
enum class SendCommandEnum : unsigned char
{
  /// State request: ask the state of the system modules
  STATE_REQUEST = 50,
  /// P/N reading (Aurora inverters)
  PN_READING = 52,
  /// Version reading (indoor/outdoor, Europe/America, available only in FW version 1.0.9 and following)
  VERSION_READING = 58,
  /// Measure request to the DSP (voltage, current, etc.)  
  MEASURE_REQUEST_DSP = 59,
  /// Serial number reading (Aurora inverters)
  SERIAL_NUMBER_READING = 63,
  /// Manufacturing week and year (Aurora inverters)
  MANUFACTURING_DATE = 65,
  /// Time/Date reading
  TIME_DATE_READING = 70,
  /// Firmware release reading
  FIRMWARE_RELEASE_READING = 72,
  /// Cumulated energy readings (Aurora grid-tied inverters only)
  CUMULATED_ENERGY_READINGS = 78,
  /// Last four alarms
  LAST_FOUR_ALARMS = 86,
  /// Command not implemented
  NOT_IMPLEMENTED = 255
};

#endif
