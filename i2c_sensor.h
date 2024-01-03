/*
 * i2c_sensor.h
 *
 *  Created on: 10.12.2023
 *      Author: Andreas Reichenauer
 */

#ifndef I2C_SENSOR_H_
#define I2C_SENSOR_H_

#include "Ifx_Types.h"

/*********************************************************************************************************************/
/*---------------------------------------------Function Definitions----------------------------------------------*/
/*********************************************************************************************************************/
/***
 * @brief: initialises the I2C master
 * @params: None
 * @returns: void
 */
void init_i2c(void);

/***
 * @brief: performs a soft reset of the sensor
 * @params: None
 * @returns: void
 */
void do_soft_reset(void);

/***
 * @brief: reads the serial number/id of the sensor
 * @params: None
 * @returns: uint32, value containing the 32 bits of the serial id
 */
uint32 read_serial_id(void);

/***
 * @brief: reads the sensor values from the sensor and converts them to floats
 * @params: float pointer, pointer used to store the converted temperature value
 * @params: float pointer, pointer used to store the converted humidity value
 * @returns: void
 */
void read_and_calculate_values(float* temperature, float* humidity);

#endif /* I2C_SENSOR_H_ */
