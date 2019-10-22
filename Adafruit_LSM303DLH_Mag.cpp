/***************************************************************************
  This is a library for the LSM303DLH Magnentometer/compass

  Designed specifically to work with the Adafruit LSM303DLHC Breakout

  These displays use I2C to communicate, 2 pins are required to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Bryan Siepert for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

#include "Adafruit_LSM303DLH_Mag.h"
#include <limits.h>

/* enabling this #define will enable the debug print blocks
#define LSM303_DEBUG
*/

static float _lsm303Mag_Gauss_LSB_XY = 1100.0F; // Varies with gain
static float _lsm303Mag_Gauss_LSB_Z = 980.0F;   // Varies with gain

/***************************************************************************
 MAGNETOMETER
 ***************************************************************************/
/***************************************************************************
 PRIVATE FUNCTIONS
 ***************************************************************************/

/**************************************************************************/
/*!
    @brief  Reads the raw data from the sensor
*/
/**************************************************************************/
void Adafruit_LSM303DLH_Mag_Unified::read() {
  // Read the magnetometer

  // Note high before low (different than accel)
  Adafruit_BusIO_Register data_reg =
      Adafruit_BusIO_Register(i2c_dev, LSM303_REGISTER_MAG_OUT_X_H_M, 6);

  uint8_t buffer[6];

  data_reg.read(buffer, 6);

  uint8_t xhi = buffer[0];
  uint8_t xlo = buffer[1];
  uint8_t zhi = buffer[2];
  uint8_t zlo = buffer[3];
  uint8_t yhi = buffer[4];
  uint8_t ylo = buffer[5];

  raw.x = (int16_t)((xhi << 8) | xlo);
  raw.y = (int16_t)((yhi << 8) | ylo);
  raw.z = (int16_t)((zhi << 8) | zlo);
}

/***************************************************************************
 PUBLIC FUNCTIONS
 ***************************************************************************/
/***************************************************************************
 CONSTRUCTOR
 ***************************************************************************/

/**************************************************************************/
/*!
    @brief  Instantiates a new Adafruit_LSM303DLH_Mag_Unified class
    @param sensorID an optional identifier for the sensor instance
*/
/**************************************************************************/
Adafruit_LSM303DLH_Mag_Unified::Adafruit_LSM303DLH_Mag_Unified(
    int32_t sensorID) {
  _sensorID = sensorID;
  autoRangeEnabled = false;

  // Clear the raw mag data
  raw.x = 0;
  raw.y = 0;
  raw.z = 0;
}

/*!
 *    @brief  Sets up the hardware and initializes I2C
 *    @param  i2c_address
 *            The I2C address to be used.
 *    @param  wire
 *            The Wire object to be used for I2C connections.
 *    @return True if initialization was successful, otherwise false.
 */
bool Adafruit_LSM303DLH_Mag_Unified::begin(uint8_t i2c_address, TwoWire *wire) {

  i2c_dev = new Adafruit_I2CDevice(i2c_address, wire);

  if (!i2c_dev->begin()) {
    return false;
  }

  // Enable the magnetometer
  Adafruit_BusIO_Register mag_mr_reg =
      Adafruit_BusIO_Register(i2c_dev, LSM303_REGISTER_MAG_MR_REG_M, 1);
  mag_mr_reg.write(0x00);

  // LSM303DLHC has no WHOAMI register, but it has IRx_REG_M that should be
  // constant
  Adafruit_BusIO_Register irx_regs =
      Adafruit_BusIO_Register(i2c_dev, LSM303_REGISTER_MAG_IRA_REG_M, 3);

  uint8_t buffer[3];
  irx_regs.read(buffer, 3);

  if (buffer[0] != 0b01001000)
    return false;
  if (buffer[1] != 0b00110100)
    return false;
  if (buffer[2] != 0b00110011)
    return false;

  setMagGain(LSM303_MAGGAIN_1_3);
  return true;
}

/**************************************************************************/
/*!
    @brief  Enables or disables auto-ranging
    @param enabled True to enable auto-ranging, false to disable
*/
/**************************************************************************/
void Adafruit_LSM303DLH_Mag_Unified::enableAutoRange(bool enabled) {
  autoRangeEnabled = enabled;
}

/**************************************************************************/
/*!
    @brief  Sets the magnetometer's gain
    @param gain a `lsm303MagGain` specifying the new gain
*/
/**************************************************************************/
void Adafruit_LSM303DLH_Mag_Unified::setMagGain(lsm303MagGain gain) {
  Adafruit_BusIO_Register crb_reg =
      Adafruit_BusIO_Register(i2c_dev, LSM303_REGISTER_MAG_CRB_REG_M, 1);

  Adafruit_BusIO_RegisterBits mag_gain =
      Adafruit_BusIO_RegisterBits(&crb_reg, 3, 5);

  mag_gain.write(gain);

  magGain = gain;

  switch (gain) {
  case LSM303_MAGGAIN_1_3:
    _lsm303Mag_Gauss_LSB_XY = 1100;
    _lsm303Mag_Gauss_LSB_Z = 980;
    break;
  case LSM303_MAGGAIN_1_9:
    _lsm303Mag_Gauss_LSB_XY = 855;
    _lsm303Mag_Gauss_LSB_Z = 760;
    break;
  case LSM303_MAGGAIN_2_5:
    _lsm303Mag_Gauss_LSB_XY = 670;
    _lsm303Mag_Gauss_LSB_Z = 600;
    break;
  case LSM303_MAGGAIN_4_0:
    _lsm303Mag_Gauss_LSB_XY = 450;
    _lsm303Mag_Gauss_LSB_Z = 400;
    break;
  case LSM303_MAGGAIN_4_7:
    _lsm303Mag_Gauss_LSB_XY = 400;
    _lsm303Mag_Gauss_LSB_Z = 355;
    break;
  case LSM303_MAGGAIN_5_6:
    _lsm303Mag_Gauss_LSB_XY = 330;
    _lsm303Mag_Gauss_LSB_Z = 295;
    break;
  case LSM303_MAGGAIN_8_1:
    _lsm303Mag_Gauss_LSB_XY = 230;
    _lsm303Mag_Gauss_LSB_Z = 205;
    break;
  }
}

/**************************************************************************/
/*!
    @brief  Sets the magnetometer's update rate
    @param  rate A `lsm303MagRate` specifying the new rate
*/
/**************************************************************************/
void Adafruit_LSM303DLH_Mag_Unified::setMagRate(lsm303MagRate rate) {

  Adafruit_BusIO_Register cra_reg =
      Adafruit_BusIO_Register(i2c_dev, LSM303_REGISTER_MAG_CRA_REG_M, 1);
  Adafruit_BusIO_RegisterBits mag_rate =
      Adafruit_BusIO_RegisterBits(&cra_reg, 3, 2);
  mag_rate.write(rate);
}

/**************************************************************************/
/*!
    @brief  Gets the most recent sensor event
    @param  event A `sensors_event_t` to store the event data
    @returns True on successful read
*/
/**************************************************************************/
bool Adafruit_LSM303DLH_Mag_Unified::getEvent(sensors_event_t *event) {
  bool readingValid = false;

  /* Clear the event */
  memset(event, 0, sizeof(sensors_event_t));
  Adafruit_BusIO_Register sr_reg =
      Adafruit_BusIO_Register(i2c_dev, LSM303_REGISTER_MAG_SR_REG_Mg, 1);

  Adafruit_BusIO_RegisterBits data_ready =
      Adafruit_BusIO_RegisterBits(&sr_reg, 1, 0);

  while (!readingValid) {

    if (!data_ready.read()) {
      return false;
    }

    /* Read new data */
    read();

    /* Make sure the sensor isn't saturating if auto-ranging is enabled */
    if (!autoRangeEnabled) {
      readingValid = true;
    } else {

      /* Check if the sensor is saturating or not */
      if ((raw.x >= 2040) | (raw.x <= -2040) | (raw.y >= 2040) |
          (raw.y <= -2040) | (raw.z >= 2040) | (raw.z <= -2040)) {
        /* Saturating .... increase the range if we can */
        switch (magGain) {
        case LSM303_MAGGAIN_5_6:
          setMagGain(LSM303_MAGGAIN_8_1);
          readingValid = false;

          break;
        case LSM303_MAGGAIN_4_7:
          setMagGain(LSM303_MAGGAIN_5_6);
          readingValid = false;

          break;
        case LSM303_MAGGAIN_4_0:
          setMagGain(LSM303_MAGGAIN_4_7);
          readingValid = false;

          break;
        case LSM303_MAGGAIN_2_5:
          setMagGain(LSM303_MAGGAIN_4_0);
          readingValid = false;

          break;
        case LSM303_MAGGAIN_1_9:
          setMagGain(LSM303_MAGGAIN_2_5);
          readingValid = false;

          break;
        case LSM303_MAGGAIN_1_3:
          setMagGain(LSM303_MAGGAIN_1_9);
          readingValid = false;

          break;
        default:
          readingValid = true;
          break;
        }
      } else {
        /* All values are withing range */
        readingValid = true;
      }
    }
  }

  event->version = sizeof(sensors_event_t);
  event->sensor_id = _sensorID;
  event->type = SENSOR_TYPE_MAGNETIC_FIELD;
  event->timestamp = millis();
  event->magnetic.x =
      (float)raw.x / _lsm303Mag_Gauss_LSB_XY * SENSORS_GAUSS_TO_MICROTESLA;
  event->magnetic.y =
      (float)raw.y / _lsm303Mag_Gauss_LSB_XY * SENSORS_GAUSS_TO_MICROTESLA;
  event->magnetic.z =
      (float)raw.z / _lsm303Mag_Gauss_LSB_Z * SENSORS_GAUSS_TO_MICROTESLA;

  return true;
}

/**************************************************************************/
/*!
    @brief  Gets the sensor_t data
*/
/**************************************************************************/
void Adafruit_LSM303DLH_Mag_Unified::getSensor(sensor_t *sensor) {
  /* Clear the sensor_t object */
  memset(sensor, 0, sizeof(sensor_t));

  /* Insert the sensor name in the fixed length char array */
  strncpy(sensor->name, "LSM303DLH Mag", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name) - 1] = 0;
  sensor->version = 1;
  sensor->sensor_id = _sensorID;
  sensor->type = SENSOR_TYPE_MAGNETIC_FIELD;
  sensor->min_delay = 0;
  sensor->max_value = 0.0F;  // TBD
  sensor->min_value = 0.0F;  // TBD
  sensor->resolution = 0.0F; // TBD
}
