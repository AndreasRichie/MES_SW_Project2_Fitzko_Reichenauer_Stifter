/*
 * oximeter5_click.h
 *
 *  Created on: 11.01.2024
 *      Author: Andreas Reichenauer
 *
 * modified version of the MikroSDK Oximeter5 Click API from GitHub
 * https://github.com/MikroElektronika/mikrosdk_click_v2/blob/master/clicks/oximeter5/lib_oximeter5/include/oximeter5.h
 */

/****************************************************************************
** Copyright (C) 2020 MikroElektronika d.o.o.
** Contact: https://www.mikroe.com/contact
**
** Permission is hereby granted, free of charge, to any person obtaining a copy
** of this software and associated documentation files (the "Software"), to deal
** in the Software without restriction, including without limitation the rights
** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
** copies of the Software, and to permit persons to whom the Software is
** furnished to do so, subject to the following conditions:
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
** OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
** DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
** OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
**  USE OR OTHER DEALINGS IN THE SOFTWARE.
****************************************************************************/

/*!
 * @file oximeter5_click.h
 * @brief This file contains API for Oximeter 5 Click Driver modified for AURIX Tricore TC257SB.
 */

#ifndef OXIMETER5_CLICK_H_
#define OXIMETER5_CLICK_H_

#include <I2c/I2c/IfxI2c_I2c.h>

#include "Ifx_Types.h"
#include "IfxPort.h"

/*!
 * @addtogroup oximeter5 Oximeter 5 Click Driver
 * @brief API for configuring and manipulating Oximeter 5 Click Driver modified for AURIX Tricore TC257SB.
 * @{
 */

/**
 * @defgroup oximeter5_reg Oximeter 5 Registers List
 * @brief List of registers of Oximeter 5 Click driver.
 */

/**
 * @addtogroup oximeter5_reg
 * @{
 */

/**
 * @brief Oximeter 5 description register.
 * @details Specified register for description of Oximeter 5 Click driver.
 */
#define OXIMETER5_REG_INTR_STATUS_1               0x00
#define OXIMETER5_REG_INTR_STATUS_2               0x01
#define OXIMETER5_REG_INTR_ENABLE_1               0x02
#define OXIMETER5_REG_INTR_ENABLE_2               0x03
#define OXIMETER5_REG_FIFO_WR_PTR                 0x04
#define OXIMETER5_REG_OVF_COUNTER                 0x05
#define OXIMETER5_REG_FIFO_RD_PTR                 0x06
#define OXIMETER5_REG_FIFO_DATA                   0x07
#define OXIMETER5_REG_FIFO_CONFIG                 0x08
#define OXIMETER5_REG_MODE_CONFIG                 0x09
#define OXIMETER5_REG_SPO2_CONFIG                 0x0A
#define OXIMETER5_REG_LED1_PA                     0x0C
#define OXIMETER5_REG_LED2_PA                     0x0D
#define OXIMETER5_REG_MULTI_LED_CTRL1             0x11
#define OXIMETER5_REG_MULTI_LED_CTRL2             0x12
#define OXIMETER5_REG_TEMP_INTR                   0x1F
#define OXIMETER5_REG_TEMP_FRAC                   0x20
#define OXIMETER5_REG_TEMP_CONFIG                 0x21
#define OXIMETER5_REG_PROX_INT_THRESH             0x30
#define OXIMETER5_REG_REV_ID                      0xFE
#define OXIMETER5_REG_PART_ID                     0xFF

/*! @} */ // oximeter5_reg

/**
 * @defgroup oximeter5_set Oximeter 5 Registers Settings
 * @brief Settings for registers of Oximeter 5 Click driver.
 */

/**
 * @addtogroup oximeter5_set
 * @{
 */

/**
 * @brief Oximeter 5 description setting.
 * @details Specified setting for description of Oximeter 5 Click driver.
 */
// OXIMETER5_REG_INTR_ENABLE_1
#define OXIMETER5_SET_INTR_EN_1_FULL_EN           0x80
#define OXIMETER5_SET_INTR_EN_1_PPG_RDY_EN        0x40
#define OXIMETER5_SET_INTR_EN_1_ALC_OVF_EN        0x20

// OXIMETER5_REG_INTR_ENABLE_2
#define OXIMETER5_SET_INTR_EN_2_TEMP_EN           0x80
#define OXIMETER5_SET_INTR_EN_2_TEMP_DIS          0x00

// OXIMETER5_REG_FIFO_CONFIG
#define OXIMETER5_SET_FIFO_PTR_RESET              0x00
#define OXIMETER5_SET_FIFO_COUNTER_RESET          0x00

// OXIMETER5_REG_FIFO_CONFIG
#define OXIMETER5_SET_FIFO_CFG_SMP_AVE_BIT_MASK   0xC0
#define OXIMETER5_SET_FIFO_CFG_SMP_AVE_1          0x00
#define OXIMETER5_SET_FIFO_CFG_SMP_AVE_2          0x20
#define OXIMETER5_SET_FIFO_CFG_SMP_AVE_3          0x40
#define OXIMETER5_SET_FIFO_CFG_SMP_AVE_8          0x60
#define OXIMETER5_SET_FIFO_CFG_SMP_AVE_16         0x80
#define OXIMETER5_SET_FIFO_CFG_SMP_AVE_32         0xC0

#define OXIMETER5_SET_FIFO_CFG_FIFO_RL_BIT_MASK   0x10
#define OXIMETER5_SET_FIFO_CFG_FIFO_RL_EN         0x10

#define OXIMETER5_SET_FIFO_CFG_DATA_SAMP_BIT_MASK 0x0F
#define OXIMETER5_SET_FIFO_CFG_DATA_SAMP_0        0x00
#define OXIMETER5_SET_FIFO_CFG_DATA_SAMP_1        0x01
#define OXIMETER5_SET_FIFO_CFG_DATA_SAMP_2        0x02
#define OXIMETER5_SET_FIFO_CFG_DATA_SAMP_3        0x03
#define OXIMETER5_SET_FIFO_CFG_DATA_SAMP_4        0x04
#define OXIMETER5_SET_FIFO_CFG_DATA_SAMP_5        0x05
#define OXIMETER5_SET_FIFO_CFG_DATA_SAMP_6        0x06
#define OXIMETER5_SET_FIFO_CFG_DATA_SAMP_7        0x07
#define OXIMETER5_SET_FIFO_CFG_DATA_SAMP_8        0x08
#define OXIMETER5_SET_FIFO_CFG_DATA_SAMP_9        0x09
#define OXIMETER5_SET_FIFO_CFG_DATA_SAMP_10       0x0A
#define OXIMETER5_SET_FIFO_CFG_DATA_SAMP_11       0x0B
#define OXIMETER5_SET_FIFO_CFG_DATA_SAMP_12       0x0C
#define OXIMETER5_SET_FIFO_CFG_DATA_SAMP_13       0x0D
#define OXIMETER5_SET_FIFO_CFG_DATA_SAMP_14       0x0E
#define OXIMETER5_SET_FIFO_CFG_DATA_SAMP_15       0x0F

#define OXIMETER5_SET_CFG_MODE_BIT_MASK           0x07
#define OXIMETER5_SW_RESET                        0x40
#define OXIMETER5_SET_CFG_MODE_HEART_RATE         0x02
#define OXIMETER5_SET_CFG_MODE_SpO2               0x03
#define OXIMETER5_SET_CFG_MODE_MULTI_LED          0x07

#define OXIMETER5_SET_SPO2_CFG_ADC_RGE_BIT_MASK   0x60
#define OXIMETER5_SET_SPO2_CFG_ADC_RGE_2048       0x00
#define OXIMETER5_SET_SPO2_CFG_ADC_RGE_4096       0x20
#define OXIMETER5_SET_SPO2_CFG_ADC_RGE_8192       0x40
#define OXIMETER5_SET_SPO2_CFG_ADC_RGE_16384      0x60

#define OXIMETER5_SET_SPO2_CFG_SR_SEC_BIT_MASK    0x1C
#define OXIMETER5_SET_SPO2_CFG_SR_SEC_50          0x00
#define OXIMETER5_SET_SPO2_CFG_SR_SEC_100         0x04
#define OXIMETER5_SET_SPO2_CFG_SR_SEC_200         0x08
#define OXIMETER5_SET_SPO2_CFG_SR_SEC_400         0x0C
#define OXIMETER5_SET_SPO2_CFG_SR_SEC_800         0x10
#define OXIMETER5_SET_SPO2_CFG_SR_SEC_1000        0x12
#define OXIMETER5_SET_SPO2_CFG_SR_SEC_1600        0x14
#define OXIMETER5_SET_SPO2_CFG_SR_SEC_3200        0x1C

#define OXIMETER5_SET_SPO2_CFG_LED_PW_BIT_MASK    0x03
#define OXIMETER5_SET_SPO2_CFG_LED_PW_15_bit      0x00
#define OXIMETER5_SET_SPO2_CFG_LED_PW_16_bit      0x01
#define OXIMETER5_SET_SPO2_CFG_LED_PW_17_bit      0x02
#define OXIMETER5_SET_SPO2_CFG_LED_PW_18_bit      0x03

#define OXIMETER5_SET_LED_PULSE_AMPL_7_2_mA       0x24

#define OXIMETER5_SET_CFG_TEMP_DISABLE            0x00
#define OXIMETER5_SET_CFG_TEMP_ENABLE             0x01

#define MAX_BRIGHTNESS                            255
#define OXIMETER5_PN_SPO2_ERROR_DATA              255
#define OXIMETER5_HEART_RATE_ERROR_DATA           -999

#define OXIMETER5_INTERRUPT_INACTIVE              0x00
#define OXIMETER5_INTERRUPT_ACTIVE                0x01

/**
 * @brief Oximeter 5 device address setting.
 * @details Specified setting for device slave address selection of
 * Oximeter 5 Click driver. This represents the 7bit I2C address.
 */
#define OXIMETER5_SET_DEV_ADDR                    0x57

/*! @} */ // oximeter5_set

// todo doc
#define SAMPLING_FREQUENCY          25
#define BUFFER_SIZE                 ( SAMPLING_FREQUENCY * 4 )

/**
 * @brief Oximeter 5 Click context object.
 * @details Context object definition of Oximeter 5 Click driver.
 */

typedef struct
{
    // Input pins
    IfxPort_Pin  int_pin;           /**< Input pin for hardware interrupt. */

    // Modules
    IfxI2c_I2c i2c;                 /**< I2C handle. */
    IfxI2c_I2c_Device i2cDev;       /**< Slave device handle. */

} oximeter5_t;

/**
 * @brief Oximeter 5 Click return value data.
 * @details Predefined enum values for driver return values.
 */
typedef enum
{
    OXIMETER5_OK = 0,
    OXIMETER5_ERROR = -1

} oximeter5_return_value_t;


/*!
 * @addtogroup oximeter5 Oximeter 5 Click Driver
 * @brief API for configuring and manipulating Oximeter 5 Click driver.
 * @{
 */

/**
 * @brief Oximeter 5 initialization function.
 * @details This function initializes all necessary pins and peripherals used
 * for this click board.
 * @param[out] ctx : Click context object.
 * See #oximeter5_t object definition for detailed explanation.
 * @return Nothing.
 *
 * @note None.
 */
void oximeter5_init ( oximeter5_t *ctx );

/**
 * @brief Oximeter 5 default configuration function.
 * @details This function executes a default configuration of Oximeter 5
 * click board.
 * @param[in] ctx : Click context object.
 * See #oximeter5_t object definition for detailed explanation.
 * @return @li @c  0 - Success,
 *         @li @c -1 - Error.
 *
 * See #oximeter5_return_value_t definition for detailed explanation.
 * @note This function can consist any necessary configuration or setting to put
 * device into operating mode.
 */
oximeter5_return_value_t oximeter5_default_cfg ( oximeter5_t *ctx );

/**
 * @brief Oximeter 5 I2C writing function.
 * @details This function writes a desired number of data bytes starting from
 * the selected register by using I2C serial interface.
 * @param[in] ctx : Click context object.
 * See #oximeter5_t object definition for detailed explanation.
 * @param[in] reg : Start register address.
 * @param[in] tx_buf : Data to be written.
 * @param[in] tx_len : Number of bytes to be written.
 * @return @li @c  0 - Success,
 *         @li @c -1 - Error.
 *
 * See #oximeter5_return_value_t definition for detailed explanation.
 * @note None.
 */
oximeter5_return_value_t oximeter5_generic_write ( oximeter5_t *ctx, uint8 reg, uint8 *tx_buf, uint8 tx_len );

/**
 * @brief Oximeter 5 I2C reading function.
 * @details This function reads a desired number of data bytes starting from
 * the selected register by using I2C serial interface.
 * @param[in] ctx : Click context object.
 * See #oximeter5_t object definition for detailed explanation.
 * @param[in] reg : Start register address.
 * @param[out] rx_buf : Output read data.
 * @param[in] rx_len : Number of bytes to be read.
 * @return @li @c  0 - Success,
 *         @li @c -1 - Error.
 *
 * See #oximeter5_return_value_t definition for detailed explanation.
 * @note None.
 */
oximeter5_return_value_t oximeter5_generic_read ( oximeter5_t *ctx, uint8 reg, uint8 *rx_buf, uint8 rx_len );

/**
 * @brief Oximeter 5 check interrupt function.
 * @details This function get states of the INT pin
 * of the MAX30102 High-Sensitivity Pulse Oximeter and
 * Heart-Rate Sensor for Wearable Health
 * on the Oximeter 5 Click board�.
 * @param[in] ctx : Click context object.
 * See #oximeter5_t object definition for detailed explanation.
 * @return @li @c 0 - Low pin state,
 *         @li @c 1 - High pin state.
 *
 * See #oximeter5_return_value_t definition for detailed explanation.
 * @note None.
 */
uint8 oximeter5_check_interrupt ( oximeter5_t *ctx );

/**
 * @brief Oximeter 5 soft reset function.
 * @details This function executes a software reset
 * of the MAX30102 High-Sensitivity Pulse Oximeter and
 * Heart-Rate Sensor for Wearable Health
 * on the Oximeter 5 Click board�.
 * @param[in] ctx : Click context object.
 * See #oximeter5_t object definition for detailed explanation.
 * @return @li @c  0 - Success,
 *         @li @c -1 - Error.
 *
 * See #oximeter5_return_value_t definition for detailed explanation.
 * @note None.
 */
oximeter5_return_value_t oximeter5_sw_reset ( oximeter5_t *ctx );

/**
 * @brief Oximeter 5 read temperature function.
 * @details This function reads a temperature data
 * of the MAX30102 High-Sensitivity Pulse Oximeter and
 * Heart-Rate Sensor for Wearable Health
 * on the Oximeter 5 Click board�.
 * @param[in] ctx : Click context object.
 * See #oximeter5_t object definition for detailed explanation.
 * @param[out] temperature : Temperature in degree Celsius.
 * @return @li @c  0 - Success,
 *         @li @c -1 - Error.
 *
 * See #oximeter5_return_value_t definition for detailed explanation.
 * @note None.
 */
oximeter5_return_value_t oximeter5_read_temperature ( oximeter5_t *ctx, float *temperature );

/**
 * @brief Oximeter 5 set FIFO write pointer function.
 * @details This function set FIFO write points to the location
 * of the MAX30102 High-Sensitivity Pulse Oximeter and
 * Heart-Rate Sensor for Wearable Health
 * on the Oximeter 5 Click board�.
 * @param[in] ctx : Click context object.
 * See #oximeter5_t object definition for detailed explanation.
 * @param[in] fifo_wr_p : FIFO write pointer points data.
 * @return @li @c  0 - Success,
 *         @li @c -1 - Error.
 *
 * See #oximeter5_return_value_t definition for detailed explanation.
 * @note This pointer advances for each sample pushed on to the FIFO.
 */
oximeter5_return_value_t oximeter5_set_fifo_wr_pointer ( oximeter5_t *ctx, uint8 fifo_wr_p );

/**
 * @brief Oximeter 5 set FIFO read pointer function.
 * @details This function set FIFO read points to the location
 * of the MAX30102 High-Sensitivity Pulse Oximeter and
 * Heart-Rate Sensor for Wearable Health
 * on the Oximeter 5 Click board�.
 * @param[in] ctx : Click context object.
 * See #oximeter5_t object definition for detailed explanation.
 * @param[in] fifo_rd_p : FIFO read pointer points data.
 * @return @li @c  0 - Success,
 *         @li @c -1 - Error.
 *
 * See #oximeter5_return_value_t definition for detailed explanation.
 * @note This pointer advances for each sample pushed on to the FIFO.
 */
oximeter5_return_value_t oximeter5_set_fifo_rd_pointer ( oximeter5_t *ctx, uint8 fifo_rd_p );

/**
 * @brief Oximeter 5 set FIFO config function.
 * @details This function set FIFO configuration
 * of the MAX30102 High-Sensitivity Pulse Oximeter and
 * Heart-Rate Sensor for Wearable Health
 * on the Oximeter 5 Click board�.
 * @param[in] ctx : Click context object.
 * See #oximeter5_t object definition for detailed explanation.
 * @param[in] smp_ave : Sample Averaging data.
 * @param[in] fifo_ro_en : FIFO Rolls on Full enable data.
 * @param[in] fifo_a_full : FIFO Almost Full Value.
 * @return @li @c  0 - Success,
 *         @li @c -1 - Error.
 *
 * See #oximeter5_return_value_t definition for detailed explanation.
 * @note None.
 */
oximeter5_return_value_t oximeter5_set_fifo_cfg ( oximeter5_t *ctx, uint8 smp_ave, uint8 fifo_ro_en, uint8 fifo_a_full );

/**
 * @brief Oximeter 5 set mode config function.
 * @details This function set mode configuration
 * of the MAX30102 High-Sensitivity Pulse Oximeter and
 * Heart-Rate Sensor for Wearable Health
 * on the Oximeter 5 Click board�.
 * @param[in] ctx : Click context object.
 * See #oximeter5_t object definition for detailed explanation.
 * @param[in] mode : Mode Control data.
 * @return @li @c  0 - Success,
 *         @li @c -1 - Error.
 *
 * See #oximeter5_return_value_t definition for detailed explanation.
 * @note None.
 */
oximeter5_return_value_t oximeter5_set_mode_cfg ( oximeter5_t *ctx, uint8 mode );

/**
 * @brief Oximeter 5 set SpO2 config function.
 * @details This function set SpO2 configuration
 * of the MAX30102 High-Sensitivity Pulse Oximeter and
 * Heart-Rate Sensor for Wearable Health
 * on the Oximeter 5 Click board�.
 * @param[in] ctx : Click context object.
 * See #oximeter5_t object definition for detailed explanation.
 * @param[in] spo2_adc_rge : SpO2 ADC Range Control data.
 * @param[in] spo2_sr : SpO2 Sample Rate Control data.
 * @param[in] led_pw : LED Pulse Width Control and ADC Resolution data.
 * @return @li @c  0 - Success,
 *         @li @c -1 - Error.
 *
 * See #oximeter5_return_value_t definition for detailed explanation.
 * @note None.
 */
oximeter5_return_value_t oximeter5_set_spo2_cfg ( oximeter5_t *ctx, uint8 spo2_adc_rge,  uint8 spo2_sr, uint8 led_pw );

/**
 * @brief Oximeter 5 get sensor data function.
 * @details This function read sensor data
 * autoincrements the register address pointer
 * of the MAX30102 High-Sensitivity Pulse Oximeter and
 * Heart-Rate Sensor for Wearable Health
 * on the Oximeter 5 Click board�.
 * @param[in] ctx : Click context object.
 * See #oximeter5_t object definition for detailed explanation.
 * @param[out] ir : IR ADC data.
 * @param[out] red : Red ADC data.
 * @return @li @c  0 - Success,
 *         @li @c -1 - Error.
 *
 * See #oximeter5_return_value_t definition for detailed explanation.
 * @note The data FIFO consists of a 32-sample memory bank that can store IR and Red ADC data.
 */
oximeter5_return_value_t oximeter5_read_sensor_data ( oximeter5_t *ctx, uint32 *ir, uint32 *red );

/**
 * @brief Oximeter 5 get oxygen saturation function.
 * @details This function get oxygen saturation data
 * autoincrements the register address pointer
 * of the MAX30102 High-Sensitivity Pulse Oximeter and
 * Heart-Rate Sensor for Wearable Health
 * on the Oximeter 5 Click board�.
 * @param[in] pun_ir_buffer : IR ADC data buffer pointer.
 * @param[in] n_ir_buffer_length : Number of IR ADC data buffer.
 * @param[in] pun_red_buffer : IR ADC data buffer pointer.
 * @param[out] pn_spo2 : SpO2 Oxygen saturation data, from 0 percent to 100 percent.
 * @return @li @c  0 - Success,
 *         @li @c -1 - Error.
 *
 * See #oximeter5_return_value_t definition for detailed explanation.
 * @note Oxygen saturation is the fraction of oxygen-saturated hemoglobin relative
 * to total hemoglobin (unsaturated + saturated) in the blood.
 * The human body requires and regulates a very precise
 * and specific balance of oxygen in the blood.
 * Normal arterial blood oxygen saturation levels in humans are 95�100 percent.
 */
oximeter5_return_value_t oximeter5_get_oxygen_saturation ( uint32 *pun_ir_buffer, sint32 n_ir_buffer_length, uint32 *pun_red_buffer, uint8 *pn_spo2 );

/**
 * @brief Oximeter 5 get heart rate function.
 * @details This function read heart rate data
 * autoincrements the register address pointer
 * of the MAX30102 High-Sensitivity Pulse Oximeter and
 * Heart-Rate Sensor for Wearable Health
 * on the Oximeter 5 Click board�.
 * @param[in] pun_ir_buffer : IR ADC data buffer pointer.
 * @param[in] n_ir_buffer_length : Number of IR ADC data buffer.
 * @param[in] pun_red_buffer : IR ADC data buffer pointer.
 * @param[out] pn_heart_rate : Heart rate data.
 * @return @li @c  0 - Success,
 *         @li @c -1 - Error.
 *
 * See #oximeter5_return_value_t definition for detailed explanation.
 * @note None.
 */
oximeter5_return_value_t oximeter5_get_heart_rate ( uint32 *pun_ir_buffer, sint32 n_ir_buffer_length, uint32 *pun_red_buffer, sint32 *pn_heart_rate );

#ifdef __cplusplus
}
#endif

#endif /* OXIMETER5_CLICK_H_ */
