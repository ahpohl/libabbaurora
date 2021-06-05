[![Build Status](https://travis-ci.com/ahpohl/libabbaurora.svg?branch=master)](https://travis-ci.com/ahpohl/libabbaurora)
# Overview

The [Aurora protocol]((docs/pdf/AuroraCommunicationProtocol_4_2.pdf)) is used by ABB to control/monitor their power devices (inverters, central inverters, etc) over a RS485 bus. Usually inverters are capable of the Aurora Protocol as well as Modbus RTU protocol. This library requires the inverter to be conneced with RS485/Aurora protocol and implements the basic functions to communicate with the inverter. It does not support other devices such as the Aurora Central or the Junction Box.

# Documentation

The library API documentation including some basic examples is located [here](insert link). The library is written in plain C++ and runs on SBCs such as the Raspberry Pi 3/4 or the Odroid C2/C4. Hence it does not need the Arduino Core for compilation as the other Arduino/esp libraries already on GitHub do.

# Related projects

The C++ library is based on the [ABBAurora Arduino library](https://github.com/jrbenito/ABBAurora) from Josenivaldo Benito Jr.

* Another [ABBAurora library](https://github.com/xreef/ABB_Aurora_Solar_Inverter_Library) for Arduino, esp8266 and esp32 by Renzo Mischianti
* Linux [Aurora](http://www.curtronics.com/Solar/AuroraData.html) command line program from Curtis Blank
* [Davide's page](http://www.drhack.it/arduino/32-lettura-inverte-power-one-aurora.html) with lots of info
* [Aurora Monitor](http://auroramonitor.sourceforge.net/) on SourceForge

# Changelog

All notable changes and releases are documented in the [CHANGELOG](CHANGELOG.md).

# License

The library is licensed under the GNU Public License - see the [LICENSE](LICENSE) file for details. The library is provided as-is without any warranty given.
