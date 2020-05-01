# Adafruit LSM303DLH Magnetometer [![Build Status](https://github.com/adafruit/Adafruit_LSM303DLH_Mag/workflows/Arduino%20Library%20CI/badge.svg)](https://github.com/adafruit/Adafruit_LSM303DLH_Mag/actions)[![Documentation](https://github.com/adafruit/ci-arduino/blob/master/assets/doxygen_badge.svg)](http://adafruit.github.io/Adafruit_LSM303DLH_Mag/html/index.html)


<a href="https://www.adafruit.com/product/1120"><img src="assets/board.jpg?raw=true" width="500px"></a>

# Adafruit LSM303DLH Magnetometer Library
This library is for the LSM303DLH's magnetometer, and is based on Adafruit's Unified Sensor Library (Adafruit_Sensor).

Tested and works great with the Adafruit's LSM303DLH Breakout Boards
* https://www.adafruit.com/product/1120

This chip uses I2C to communicate, 2 pins are required to interface

Adafruit invests time and resources providing this open source code, please support Adafruit and open-source hardware by purchasing products from Adafruit!

## About the LSM303 DLH

The LSM303 is a digital (I2C) accelerometer and digital compass (magnetometer) and accelerometer. The magnetometer measures magnetic force, which is useful to detect magnetic north.

More information on the LSM303DLH can be found in the datasheet: http://www.adafruit.com/datasheets/LSM303DLHC.PDF

## What is the Adafruit Unified Sensor Library? ##

The Adafruit Unified Sensor Library (**Adafruit_Sensor**) provides a common interface and data type for any supported sensor.  It defines some basic information about the sensor (sensor limits, etc.), and returns standard SI units of a specific type and scale for each supported sensor type.

It provides a simple abstraction layer between your application and the actual sensor HW, allowing you to drop in any comparable sensor with only one or two lines of code to change in your project (essentially the constructor since the functions to read sensor data and get information about the sensor are defined in the base Adafruit_Sensor class).

This is imporant useful for two reasons:

1.) You can use the data right away because it's already converted to SI units that you understand and can compare, rather than meaningless values like 0..1023.

2.) Because SI units are standardised in the sensor library, you can also do quick sanity checks working with new sensors, or drop in any comparable sensor if you need better sensitivity or if a lower cost unit becomes available, etc. 

Light sensors will always report units in lux, gyroscopes will always report units in rad/s, etc. ... freeing you up to focus on the data, rather than digging through the datasheet to understand what the sensor's raw numbers really mean.

# Installation
To install, use the Arduino Library Manager and search for "Adafruit LSM303DLH Mag" and install the library.

## Dependencies
 * [Adafruit BusIO](https://github.com/adafruit/Adafruit_BusIO)
 * [Adafruit Unified Sensor Driver](https://github.com/adafruit/Adafruit_Sensor)

# Contributing

Contributions are welcome! Please read our [Code of Conduct](https://github.com/adafruit/Adafruit_LSM303DLH_Mag/blob/master/CODE_OF_CONDUCT.md>)
before contributing to help this project stay welcoming.

## Documentation and doxygen
Documentation is produced by doxygen. Contributions should include documentation for any new code added.

Some examples of how to use doxygen can be found in these guide pages:

https://learn.adafruit.com/the-well-automated-arduino-library/doxygen

https://learn.adafruit.com/the-well-automated-arduino-library/doxygen-tips

## Formatting and clang-format
This library uses [`clang-format`](https://releases.llvm.org/download.html) to standardize the formatting of `.cpp` and `.h` files. 
Contributions should be formatted using `clang-format`:

The `-i` flag will make the changes to the file.
```bash
clang-format -i *.cpp *.h
```
If you prefer to make the changes yourself, running `clang-format` without the `-i` flag will print out a formatted version of the file. You can save this to a file and diff it against the original to see the changes.

Note that the formatting output by `clang-format` is what the automated formatting checker will expect. Any diffs from this formatting will result in a failed build until they are addressed. Using the `-i` flag is highly recommended.

### clang-format resources
  * [Binary builds and source available on the LLVM downloads page](https://releases.llvm.org/download.html)
  * [Documentation and IDE integration](https://clang.llvm.org/docs/ClangFormat.html)

## About this Driver
Written by Bryan Siepert for Adafruit Industries.
BSD license, check license.txt for more information
All text above must be included in any redistribution

