# A C++ library for the SparkFun 3-axis magnetometer breakout board

The MAG3110 library has been derived from the [Sparkfun Arduino library][1] and ported to C++ to allow embedded devices such as the Raspberry Pi with an I<SUP>2</SUP>C bus to use the breakout board directly. It uses much of the same API as the original library. However, a couple of methods have been renamed, added or removed. The complete [documentation][2] of the MAG3110 class and its methods can be found [here][2] including some basic examples to get you started.

Freescale’s MAG3110 is a small, low-power, digital 3-axis magnetometer. The device can be used in conjunction with a 3-axis accelerometer to realize an orientation independent electronic compass that can provide accurate heading information. It features a standard I<SUP>2</SUP>C serial interface output and is capable of measuring magnetic fields with an output data rate (ODR) up to 80 Hz; these output data rates correspond to sample intervals from 12.5 ms to several seconds. For details see the [technical reference][3] documentation.

[1]: https://github.com/sparkfun/SparkFun_MAG3110_Breakout_Board_Arduino_Library "MAG3110 Sparkfun Arduino Library"
[2]: https://ahpohl.github.io/mag3110/ "MAG3110 C++ Library Reference Documentation"
[3]: docs/pdf/MAG3110_reference.pdf "MAG3110 Hardware Technical Reference Documentation"

## License

The library is licensed under the GNU Public License - see the [LICENSE](LICENSE) file for details. The library is provided as-is without any warranty given.
