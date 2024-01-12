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
 * @brief Oximeter 5 hardware startup function.
 * @details This function initializes all necessary pins and peripherals used
 * for this click board, resets the sensor, sets all operation modes and does
 * an initial reading.
 * @params: None.
 * @return @li @c  0 - Success,
 *         @li @c -1 - Error.
 *
 * See #oximeter5_return_value_t definition for detailed explanation.
 * @note None.
 */
oximeter5_return_value_t prepare_oximeter5_hardware();

/**
 * @brief Oximeter 5 continuous reading function.
 * @details This function continuously reads from the Oximeter 5 and saves the
 * values to a shared memory till an error occurs.
 * @params: None.
 * @return @li @c  0 - Success,
 *         @li @c -1 - Error.
 *
 * See #oximeter5_return_value_t definition for detailed explanation.
 * @note None.
 */
oximeter5_return_value_t read_and_save_values();

#endif /* HR_AND_SPO2_HANDLER_H_ */
