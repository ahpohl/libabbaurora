# Overview

The [Aurora protocol]((./docs/pdf/AuroraCommunicationProtocol_4_2.pdf)) is used by ABB to control/monitor their power devices (inverters, central inverters, etc) over a RS485 bus. Usually inverters are capable of the Aurora Protocol as well as Modbus RTU protocol. This library requires the inverter to be conneced with RS485/Aurora protocol and implements the basic functions to communicate with the inverter. It does not support other devices such as the Aurora Central or the Junction Box. 

# Documentation

The library API documentation including some basic examples is located here.

# Related projects

* [ABBAurora Arduino library](https://github.com/jrbenito/ABBAurora) by Josenivaldo Benito Jr
* [Davide's page](http://www.drhack.it/arduino/32-lettura-inverte-power-one-aurora.html).
* [Another Arduino library](https://github.com/xreef/ABB_Aurora_Solar_Inverter_Library) by Renzo Mischianti
* [Linux Aurora program](http://www.curtronics.com/Solar/AuroraData.html) from Curtis Blank
* [aurora monitor](http://auroramonitor.sourceforge.net/)

# Changelog

All notable changes and releases are documented in the [CHANGELOG](CHANGELOG.md).

# License

The library is licensed under the GNU Public License - see the [LICENSE](LICENSE) file for details. The library is provided as-is without any warranty given.
