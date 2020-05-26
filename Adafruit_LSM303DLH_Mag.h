/*!
 * @file Adafruit_LSM303DLH_Mag.h
 *
 */

#ifndef LSM303DLH_MAG_H
#define LSM303DLH_MAG_H

#if (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

/*!
 * @brief I2C Address
 */
#define _ADDRESS_MAG 0x1E

/*!
 * @brief I2C bits
 */
typedef enum {
  LSM303_REGISTER_MAG_CRA_REG_M = 0x00,
  LSM303_REGISTER_MAG_CRB_REG_M = 0x01,
  LSM303_REGISTER_MAG_MR_REG_M = 0x02,
  LSM303_REGISTER_MAG_OUT_X_H_M = 0x03,
  LSM303_REGISTER_MAG_OUT_X_L_M = 0x04,
  LSM303_REGISTER_MAG_OUT_Z_H_M = 0x05,
  LSM303_REGISTER_MAG_OUT_Z_L_M = 0x06,
  LSM303_REGISTER_MAG_OUT_Y_H_M = 0x07,
  LSM303_REGISTER_MAG_OUT_Y_L_M = 0x08,
  LSM303_REGISTER_MAG_SR_REG_Mg = 0x09,
  LSM303_REGISTER_MAG_IRA_REG_M = 0x0A,
  LSM303_REGISTER_MAG_IRB_REG_M = 0x0B,
  LSM303_REGISTER_MAG_IRC_REG_M = 0x0C,
  LSM303_REGISTER_MAG_TEMP_OUT_H_M = 0x31,
  LSM303_REGISTER_MAG_TEMP_OUT_L_M = 0x32
} lsm303MagRegisters_t;
/*=========================================================================*/

/*!
 * @brief Magnetometer gain settings
 */
typedef enum {
  LSM303_MAGGAIN_1_3, // +/- 1.3
  LSM303_MAGGAIN_1_9, // +/- 1.9
  LSM303_MAGGAIN_2_5, // +/- 2.5
  LSM303_MAGGAIN_4_0, // +/- 4.0
  LSM303_MAGGAIN_4_7, // +/- 4.7
  LSM303_MAGGAIN_5_6, // +/- 5.6
  LSM303_MAGGAIN_8_1  // +/- 8.1
} lsm303MagGain;
/*=========================================================================*/

/*!
 * @brief Magnetometer update rate settings
 */
typedef enum {
  LSM303_MAGRATE_0_7, // 0.75 Hz
  LSM303_MAGRATE_1_5, // 1.5 Hz
  LSM303_MAGRATE_3_0, // 3.0 Hz
  LSM303_MAGRATE_7_5, // 7.5 Hz
  LSM303_MAGRATE_15,  // 15 Hz
  LSM303_MAGRATE_30,  // 30 Hz
  LSM303_MAGRATE_75,  // 75 Hz
  LSM303_MAGRATE_220  // 220 Hz
} lsm303MagRate;
/*=========================================================================*/

/**************************************************************************/
/*!
    @brief  INTERNAL ACCELERATION DATA TYPE
*/
/**************************************************************************/
typedef struct lsm303MagData_s {
  int16_t x; ///< x-axis data
  int16_t y; ///< y-axis data
  int16_t z; ///< z-axis data
} lsm303MagData;
/*=========================================================================*/

#define LSM303_ID (0b11010100) //!< Chip ID
/*=========================================================================*/

/*!
  @brief Unified sensor driver for the magnetometer
*/
class Adafruit_LSM303DLH_Mag_Unified : public Adafruit_Sensor {
public:
  Adafruit_LSM303DLH_Mag_Unified(int32_t sensorID = -1);

  bool begin(uint8_t i2c_addr = _ADDRESS_MAG, TwoWire *wire = &Wire);

  void enableAutoRange(bool enable);
  void setMagGain(lsm303MagGain gain);
  void setMagRate(lsm303MagRate rate);
  bool getEvent(sensors_event_t *);
  void getSensor(sensor_t *);

  lsm303MagData raw;     ///< Last read magnetometer data will be available here
  lsm303MagGain magGain; ///< The current magnetometer gain
  bool autoRangeEnabled; ///< True if auto ranging is enabled

private:
  int32_t _sensorID;

  void write8(byte address, byte reg, byte value);
  byte read8(byte address, byte reg);
  void read(void);
  Adafruit_I2CDevice *i2c_dev;
};

#endif
