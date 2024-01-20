/*
 * hr_and_spo2_handler.h
 *
 *  Created on: 11.01.2024
 *      Author: Andreas Reichenauer
 */

/*!
 * @file oximeter5_click.h
 * @brief This file handles interfacing with the Oximeter 5 Click Driver and saves the obtained values in a shared memory.
 */

#ifndef HR_AND_SPO2_HANDLER_H_
#define HR_AND_SPO2_HANDLER_H_

#include "oximeter5_click.h"

/**
 * @brief Hardware Interface return value data.
 * @details Predefined enum values for hardware interface return values.
 */
typedef enum
{
    SUCCESS = 0,
    SENSOR_ERROR = -1,
    CALCULATION_ERROR = -2,
    SAVE_ERROR = -3,
    LOAD_ERROR = -4

} interface_return_value_t;

/**
 * @brief Oximeter 5 hardware startup function.
 * @details This function initializes all necessary pins and peripherals used
 * for this click board, resets the sensor, sets all operation modes and does
 * an initial reading.
 * @param[in] oximeter5 : Oximeter 5 Click context object.
 * @param[in] ir_buffer : Array to store the read IR brightness values in.
 * @param[in] red_buffer : Array to store the read red brightness values in.
 * @return @li @c  0 - Success,
 *         @li @c -1 - Error Oximeter 5,
 *         @li @c -2 - Error calculating values,
 *         @li @c -3 - Error saving values,
 *
 * See #interface_return_value_t definition for detailed explanation.
 * @note None.
 */
interface_return_value_t prepare_oximeter5_hardware(oximeter5_t *oximeter5, uint32 *ir_buffer, uint32 *red_buffer);

/**
 * @brief Oximeter 5 reading and saving function.
 * @details This function reads the brightness values from the Oximeter 5 and
 * calculates spo2 and heart rate values based on read samples. The calculated
 * values are stored to the shared memory.
 * @param[in] oximeter5 : Oximeter 5 Click context object.
 * @param[in] ir_buffer : Array to store the read IR brightness values in.
 * @param[in] red_buffer : Array to store the read red brightness values in.
 * @return @li @c  0 - Success,
 *         @li @c -1 - Error Oximeter 5,
 *         @li @c -2 - Error calculating values,
 *         @li @c -3 - Error saving values,
 *
 * See #interface_return_value_t definition for detailed explanation.
 * @note None.
 */
interface_return_value_t read_and_calculate_values(oximeter5_t *oximeter5, uint32 *ir_buffer, uint32 *red_buffer);

/**
 * @brief Oximeter 5 get values function.
 * @details This function retrieves the calculated spo2 and heart rate values
 * from the shared memory.
 * @param[out] spo2 : SPO2 value stored in shared memory.
 * @param[out] heart_rate : heart rate value stored in shared memory.
 * @return @li @c  0 - Success,
 *         @li @c -1 - Error Oximeter 5,
 *         @li @c -2 - Error calculating values,
 *         @li @c -3 - Error saving values,
 *
 * See #interface_return_value_t definition for detailed explanation.
 * @note None.
 */
interface_return_value_t get_values(uint8 *spo2, sint32 *heart_rate);

#endif /* HR_AND_SPO2_HANDLER_H_ */
