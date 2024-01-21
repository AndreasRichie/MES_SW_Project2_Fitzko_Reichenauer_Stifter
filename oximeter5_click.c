/*
 * oximeter5_click.c
 *
 *  Created on: 11.01.2024
 *      Author: Andreas Reichenauer
 *
 * modified version of the MikroSDK Oximeter5 Click API from GitHub
 * https://github.com/MikroElektronika/mikrosdk_click_v2/blob/master/clicks/oximeter5/lib_oximeter5/src/oximeter5.c
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
 * @file oximeter5_click.c
 * @brief Oximeter 5 Click Driver modified for AURIX Tricore TC257SB.
 */

#include "oximeter5_click.h"
#include <Bsp.h>                      //Board support functions (for the waitTime function)

#define I2C_FREQ                    400000      // Clock frequency of I2C in Hz
#define MA4_SIZE                    4
#define DATA_18_BIT                 0x03FFFF
#define MAX_UNSIGNED_8_BIT_DATA     0x7F
#define DATA_CONV_SIGN_8_BIT_DATA   256
#define BYTE_LOW_NIBBLE             0x0F
#define TEMPERATURE_DATA_CALC_DATA  0.0625
#define OXIMETER5_N_X_DC_MAX        -16777216
#define TX_BUFFER_SIZE              257

const uint8 uch_spo2_table[ 184 ] =
{
    95, 95, 95, 96, 96, 96, 97, 97, 97, 97, 97, 98, 98, 98, 98, 98, 99, 99, 99, 99,
    99, 99, 99, 99, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
    100, 100, 100, 100, 100, 100, 100, 99, 99, 99, 99, 99, 99, 99, 99, 98, 98, 98,
    98, 98, 98, 97, 97, 97, 97, 96, 96, 96, 96, 95, 95, 95, 94, 94, 94, 93, 93, 93,
    92, 92, 92, 91, 91, 90, 90, 89, 89, 89, 88, 88, 87, 87, 86, 86, 85, 85, 84, 84,
    83, 82, 82, 81, 81, 80, 80, 79, 78, 78, 77, 76, 76, 75, 74, 74, 73, 72, 72, 71,
    70, 69, 69, 68, 67, 66, 66, 65, 64, 63, 62, 62, 61, 60, 59, 58, 57, 56, 56, 55,
    54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35,
    34, 33, 31, 30, 29, 28, 27, 26, 25, 23, 22, 21, 20, 19, 17, 16, 15, 14, 12, 11,
    10, 9, 7, 6, 5, 3, 2, 1
};

/**
 * @brief Oximeter 5 find peaks above n_min_height function.
 * @details This function find all peaks above MIN_HEIGHT.
 */
static void dev_peaks_above_min_height ( sint32 *pn_locs, sint32 *n_npks,  sint32  *pn_x, uint8 n_size, sint32 n_min_height );

/**
 * @brief Oximeter 5 sort indices function.
 * @details This function sort indices according to descending order ( insertion sort algorithm ).
 */
static void dev_sort_indices_descend ( sint32 *pn_x, sint32 *pn_indx, sint32 n_size );

/**
 * @brief Oximeter 5 sort array function.
 * @details This function array in ascending order ( insertion sort algorithm ).
 */
static void dev_sort_ascend ( sint32  *pn_x, sint32 n_size );

/**
 * @brief Oximeter 5 remove peaks function.
 * @details This function remove peaks separated by less than MIN_DISTANCE.
 */
static void dev_remove_close_peaks ( sint32 *pn_locs, sint32 *pn_npks, sint32 *pn_x, sint32 n_min_distance );

/**
 * @brief Oximeter 5 find peaks function.
 * @details This function find at most MAX_NUM peaks above MIN_HEIGHT separated by at least MIN_DISTANCE.
 */
static void dev_find_peaks ( sint32 *pn_locs, sint32 *n_npks,  sint32  *pn_x, uint8 n_size, sint32 n_min_height, sint32 n_min_distance, sint32 n_max_num );

/**
 * @brief Delay execution for 10ms function.
 * @details This function delays the execution of the program for 10ms.
 */
static void Delay_10ms ( void );

/**
 * @brief Delay execution for 1s function.
 * @details This function delays the execution of the program for 1s.
 */
static void Delay_1sec ( void );

void oximeter5_init ( oximeter5_t *ctx )
{
    // Hardware interrupt pin of CON1
    ctx->int_pin.port = &MODULE_P20;
    ctx->int_pin.pinIndex = 0;

    // Define hw int pin as input with pull up resistor
    IfxPort_setPinModeInput(ctx->int_pin.port, ctx->int_pin.pinIndex, IfxPort_InputMode_pullUp);

    IfxI2c_I2c_Config config;                               // Create config structure
    IfxI2c_I2c_initConfig(&config, &MODULE_I2C0);           // Fill structure with default values and Module address

    // Configure pins
    const IfxI2c_Pins pins = {
        &IfxI2c0_SCL_P02_5_INOUT,
        &IfxI2c0_SDA_P02_4_INOUT,
        IfxPort_PadDriver_cmosAutomotiveSpeed1
    };

    config.pins = &pins;                                         // Set configured Pins to config
    config.baudrate = I2C_FREQ;                                  // Set I2C clock frequency to config

    IfxI2c_I2c_initModule(&ctx->i2c, &config);                   // Initialize module

    IfxI2c_I2c_deviceConfig i2cDeviceConfig;                     // Create device config
    IfxI2c_I2c_initDeviceConfig(&i2cDeviceConfig, &ctx->i2c);    // Fill structure with default values and i2c Handler

    // Convert 7 bit device address into 8 bit value
    uint8 address = OXIMETER5_SET_DEV_ADDR;
    i2cDeviceConfig.deviceAddress = address << 1;

    IfxI2c_I2c_initDevice(&ctx->i2cDev, &i2cDeviceConfig);       // Initialize the i2c device handle
}

oximeter5_return_value_t oximeter5_default_cfg ( oximeter5_t *ctx )
{
    uint8 tmp;

    oximeter5_return_value_t error_flag = oximeter5_sw_reset( ctx );
    Delay_1sec( );

    error_flag |= oximeter5_generic_read( ctx, OXIMETER5_REG_INTR_STATUS_1, &tmp, 1 );
    Delay_10ms( );

    tmp = OXIMETER5_SET_INTR_EN_1_FULL_EN;
    tmp |= OXIMETER5_SET_INTR_EN_1_PPG_RDY_EN;
    error_flag |= oximeter5_generic_write( ctx, OXIMETER5_REG_INTR_ENABLE_1, &tmp, 1 );
    Delay_10ms( );

    tmp = OXIMETER5_SET_INTR_EN_2_TEMP_DIS;
    error_flag |= oximeter5_generic_write( ctx, OXIMETER5_REG_INTR_ENABLE_2, &tmp, 1 );
    Delay_10ms( );

    tmp = OXIMETER5_SET_FIFO_PTR_RESET;
    error_flag |= oximeter5_generic_write( ctx, OXIMETER5_REG_FIFO_WR_PTR, &tmp, 1 );
    Delay_10ms( );

    tmp = OXIMETER5_SET_FIFO_COUNTER_RESET;
    error_flag |= oximeter5_generic_write( ctx, OXIMETER5_REG_OVF_COUNTER, &tmp, 1 );
    Delay_10ms( );

    tmp = OXIMETER5_SET_FIFO_PTR_RESET;
    error_flag |= oximeter5_generic_write( ctx, OXIMETER5_REG_FIFO_RD_PTR, &tmp, 1 );
    Delay_10ms( );

    tmp = OXIMETER5_SET_FIFO_CFG_SMP_AVE_3;
    tmp |= OXIMETER5_SET_FIFO_CFG_DATA_SAMP_15;
    error_flag |= oximeter5_generic_write( ctx, OXIMETER5_REG_FIFO_CONFIG, &tmp, 1 );
    Delay_10ms( );

    tmp = OXIMETER5_SET_CFG_MODE_SpO2;
    error_flag |= oximeter5_generic_write( ctx, OXIMETER5_REG_MODE_CONFIG, &tmp, 1 );
    Delay_10ms( );

    tmp = OXIMETER5_SET_SPO2_CFG_ADC_RGE_4096;
    tmp |= OXIMETER5_SET_SPO2_CFG_SR_SEC_100;
    tmp |= OXIMETER5_SET_SPO2_CFG_LED_PW_18_bit;
    error_flag |= oximeter5_generic_write( ctx, OXIMETER5_REG_SPO2_CONFIG, &tmp, 1 );
    Delay_10ms( );

    tmp = OXIMETER5_SET_LED_PULSE_AMPL_7_2_mA;
    error_flag |= oximeter5_generic_write( ctx, OXIMETER5_REG_LED1_PA, &tmp, 1 );
    Delay_10ms( );

    tmp = OXIMETER5_SET_LED_PULSE_AMPL_7_2_mA;
    error_flag |= oximeter5_generic_write( ctx, OXIMETER5_REG_LED2_PA, &tmp, 1 );
    Delay_10ms( );

    uint32 ir, red;
    error_flag = oximeter5_read_sensor_data( ctx, &ir, &red );

    return error_flag;
}

oximeter5_return_value_t oximeter5_generic_write ( oximeter5_t *ctx, uint8 reg, uint8 *tx_buf, uint8 tx_len )
{
    // reserve TX buffer
    uint8 data_buf[TX_BUFFER_SIZE] = {0};

    // set register to write to as first byte
    data_buf[0] = reg;

    // copy TX data to TX buffer
    for (uint8 cnt = 1; cnt <= tx_len; cnt++)
        data_buf[cnt] = tx_buf[cnt - 1];

    // Write TX buffer to device as soon as it is ready
    IfxI2c_I2c_Status i2c_status = IfxI2c_I2c_Status_nak;
    while(i2c_status == IfxI2c_I2c_Status_nak)
        i2c_status = IfxI2c_I2c_write(&ctx->i2cDev, data_buf, tx_len+1);

    return (i2c_status == IfxI2c_I2c_Status_ok) ? OXIMETER5_OK : OXIMETER5_ERROR;
}

oximeter5_return_value_t oximeter5_generic_read ( oximeter5_t *ctx, uint8 reg, uint8 *rx_buf, uint8 rx_len )
{
    // send register to read from
    if (oximeter5_generic_write(ctx, reg, NULL_PTR, 0) == OXIMETER5_ERROR)
        return OXIMETER5_ERROR;

    // Wait after register was sent before read is started
    Delay_10ms( );

    // Read device data to data array
    IfxI2c_I2c_Status i2c_status = IfxI2c_I2c_Status_nak;
    while(i2c_status == IfxI2c_I2c_Status_nak)
        i2c_status = IfxI2c_I2c_read(&ctx->i2cDev, rx_buf, rx_len);

    return (i2c_status == IfxI2c_I2c_Status_ok) ? OXIMETER5_OK : OXIMETER5_ERROR;
}

uint8 oximeter5_check_interrupt ( oximeter5_t *ctx )
{
    return IfxPort_getPinState(ctx->int_pin.port, ctx->int_pin.pinIndex);
}

oximeter5_return_value_t oximeter5_sw_reset ( oximeter5_t *ctx )
{
    uint8 tx_data;

    tx_data = OXIMETER5_SW_RESET;
    oximeter5_return_value_t error_flag = oximeter5_generic_write( ctx, OXIMETER5_REG_MODE_CONFIG, &tx_data, 1 );

    return error_flag;
}

oximeter5_return_value_t oximeter5_read_temperature ( oximeter5_t *ctx, float *temperature )
{
    uint8 tx_data;
    uint8 rx_data;

    tx_data = OXIMETER5_SET_CFG_TEMP_ENABLE;
    oximeter5_return_value_t error_flag = oximeter5_generic_write( ctx, OXIMETER5_REG_TEMP_CONFIG, &tx_data, 1 );
    Delay_10ms( );

    error_flag |= oximeter5_generic_read( ctx, OXIMETER5_REG_TEMP_INTR, &rx_data, 1 );

    *temperature = ( float ) rx_data;
    if ( rx_data > MAX_UNSIGNED_8_BIT_DATA )
    {
        *temperature -= DATA_CONV_SIGN_8_BIT_DATA;
    }

    error_flag |= oximeter5_generic_read( ctx, OXIMETER5_REG_TEMP_FRAC, &rx_data, 1 );
    rx_data &= BYTE_LOW_NIBBLE;

    *temperature += ( float ) rx_data * TEMPERATURE_DATA_CALC_DATA;

    return error_flag;
}

oximeter5_return_value_t oximeter5_set_fifo_wr_pointer ( oximeter5_t *ctx, uint8 fifo_wr_p )
{
    return oximeter5_generic_write( ctx, OXIMETER5_REG_FIFO_WR_PTR, &fifo_wr_p, 1 );
}

oximeter5_return_value_t oximeter5_set_fifo_rd_pointer ( oximeter5_t *ctx, uint8 fifo_rd_p )
{
    return oximeter5_generic_write( ctx, OXIMETER5_REG_FIFO_RD_PTR, &fifo_rd_p, 1 );
}

oximeter5_return_value_t oximeter5_set_fifo_cfg ( oximeter5_t *ctx, uint8 smp_ave, uint8 fifo_ro_en, uint8 fifo_a_full )
{
    uint8 tx_data;

    tx_data = smp_ave & OXIMETER5_SET_FIFO_CFG_SMP_AVE_BIT_MASK;
    tx_data |= fifo_ro_en & OXIMETER5_SET_FIFO_CFG_FIFO_RL_BIT_MASK;
    tx_data |= smp_ave & OXIMETER5_SET_FIFO_CFG_DATA_SAMP_BIT_MASK;

    return oximeter5_generic_write( ctx, OXIMETER5_REG_FIFO_CONFIG, &tx_data, 1 );
}

oximeter5_return_value_t oximeter5_set_mode_cfg ( oximeter5_t *ctx, uint8 mode )
{
    mode &= OXIMETER5_SET_CFG_MODE_BIT_MASK;

    return oximeter5_generic_write( ctx, OXIMETER5_REG_MODE_CONFIG, &mode, 1 );
}

oximeter5_return_value_t oximeter5_set_spo2_cfg ( oximeter5_t *ctx, uint8 spo2_adc_rge,  uint8 spo2_sr, uint8 led_pw )
{
    uint8 tx_data;

    tx_data = spo2_adc_rge & OXIMETER5_SET_SPO2_CFG_ADC_RGE_BIT_MASK;
    tx_data |= spo2_sr & OXIMETER5_SET_SPO2_CFG_SR_SEC_BIT_MASK;
    tx_data |= led_pw & OXIMETER5_SET_SPO2_CFG_LED_PW_BIT_MASK;

    return oximeter5_generic_write( ctx, OXIMETER5_REG_MODE_CONFIG, &tx_data, 1 );
}

oximeter5_return_value_t oximeter5_read_sensor_data ( oximeter5_t *ctx, uint32 *ir, uint32 *red )
{
    uint8 rx_buf[ 6 ];

    oximeter5_return_value_t error_flag = oximeter5_generic_read( ctx, OXIMETER5_REG_FIFO_DATA, rx_buf, 6 );

    *ir = rx_buf[ 0 ];
    *ir <<= 8;
    *ir |= rx_buf[ 1 ];
    *ir <<= 8;
    *ir |= rx_buf[ 2 ];
    *ir &= DATA_18_BIT;

    *red = rx_buf[ 3 ];
    *red <<= 8;
    *red |= rx_buf[ 4 ];
    *red <<= 8;
    *red |= rx_buf[ 5 ];
    *red &= DATA_18_BIT;

    return error_flag;
}

oximeter5_return_value_t oximeter5_get_oxygen_saturation ( uint32 *pun_ir_buffer, sint32 n_ir_buffer_length, uint32 *pun_red_buffer, uint8 *pn_spo2 )
{
    uint32 un_ir_mean;
    sint32 n_i_ratio_count;
    sint32 n_exact_ir_valley_locs_count, n_middle_idx;
    sint32 n_th1, n_npks;
    sint32 an_ir_valley_locs[ 15 ];
    sint32 n_y_ac, n_x_ac;
    sint32 n_spo2_calc;
    sint32 n_y_dc_max, n_x_dc_max;
    sint32 n_y_dc_max_idx, n_x_dc_max_idx;
    sint32 an_ratio[ 5 ], n_ratio_average;
    sint32 n_nume, n_denom ;
    sint32 an_x[ BUFFER_SIZE ];
    sint32 an_y[ BUFFER_SIZE ];
    oximeter5_return_value_t error_flag;

    un_ir_mean = 0;

    for ( sint32 n_cnt_k = 0; n_cnt_k < n_ir_buffer_length; n_cnt_k++ )
    {
        un_ir_mean += pun_ir_buffer[ n_cnt_k ];
    }

    un_ir_mean =un_ir_mean/n_ir_buffer_length ;

    // remove DC and invert signal so that we can use peak detector as valley detector
    for ( sint32 n_cnt_k = 0; n_cnt_k < n_ir_buffer_length; n_cnt_k++ )
    {
        an_x[ n_cnt_k ] = -1*(pun_ir_buffer[ n_cnt_k ] - un_ir_mean );
    }

    // 4 pt Moving Average
    for ( sint32 n_cnt_k = 0; n_cnt_k < BUFFER_SIZE-MA4_SIZE; n_cnt_k++ )
    {
        an_x[ n_cnt_k ] = ( an_x[ n_cnt_k ] + an_x[ n_cnt_k + 1 ] + an_x[ n_cnt_k + 2 ] + an_x[ n_cnt_k + 3 ] ) / 4;
    }

    // calculate threshold
    n_th1 = 0;
    for ( sint32 n_cnt_k = 0; n_cnt_k < BUFFER_SIZE; n_cnt_k++ )
    {
        n_th1 +=  an_x[ n_cnt_k ];
    }

    n_th1=  n_th1 / BUFFER_SIZE;
    if ( n_th1 < 30 )
    {
        n_th1 = 30; // min allowed
    }

    if ( n_th1 > 60 )
    {
        n_th1 = 60; // max allowed
    }

    for ( sint32 n_cnt_k = 0; n_cnt_k < 15; n_cnt_k++ )
    {
        an_ir_valley_locs[ n_cnt_k ] = 0;
    }

    // since we flipped signal, we use peak detector as valley detector
    dev_find_peaks( an_ir_valley_locs, &n_npks, an_x, BUFFER_SIZE, n_th1, 4, 15 );//peak_height, peak_distance, max_num_peaks

    //  load raw value again for SPO2 calculation : RED(=y) and IR(=X)
    for ( sint32 n_cnt_k = 0; n_cnt_k < n_ir_buffer_length; n_cnt_k++ )
    {
        an_x[ n_cnt_k ] = pun_ir_buffer[ n_cnt_k ];
        an_y[ n_cnt_k ] = pun_red_buffer[ n_cnt_k ];
    }

    // find precise min near an_ir_valley_locs
    n_exact_ir_valley_locs_count = n_npks;

    //using exact_ir_valley_locs , find ir-red DC andir-red AC for SPO2 calibration an_ratio
    //finding AC/DC maximum of raw
    n_ratio_average = 0;
    n_i_ratio_count = 0;

    for ( sint32 n_cnt_k = 0; n_cnt_k < 5; n_cnt_k++ )
    {
        an_ratio[ n_cnt_k ] = 0;
    }

    for ( sint32 n_cnt_k = 0; n_cnt_k < n_exact_ir_valley_locs_count; n_cnt_k++ )
    {
        if ( an_ir_valley_locs[ n_cnt_k ] > BUFFER_SIZE )
        {
            // do not use SPO2 since valley loc is out of range
            *pn_spo2 = OXIMETER5_PN_SPO2_ERROR_DATA;
            error_flag  = OXIMETER5_ERROR;
        }
    }

    // find max between two valley locations
    // and use an_ratio betwen AC compoent of Ir & Red and DC compoent of Ir & Red for SPO2
    for ( sint32 n_cnt_k = 0; n_cnt_k < n_exact_ir_valley_locs_count - 1; n_cnt_k++ )
    {
        n_y_dc_max= OXIMETER5_N_X_DC_MAX;
        n_x_dc_max= OXIMETER5_N_X_DC_MAX;

        if ( an_ir_valley_locs[ n_cnt_k + 1 ] - an_ir_valley_locs[ n_cnt_k ] > 3 )
        {
            for ( sint32 n_cnt_i = an_ir_valley_locs[ n_cnt_k]; n_cnt_i < an_ir_valley_locs[ n_cnt_k + 1 ]; n_cnt_i++ )
            {
                if ( an_x[ n_cnt_i ] > n_x_dc_max )
                {
                    n_x_dc_max = an_x[ n_cnt_i ];
                    n_x_dc_max_idx = n_cnt_i;
                }

                if ( an_y[ n_cnt_i ] > n_y_dc_max )
                {
                    n_y_dc_max = an_y[ n_cnt_i ];
                    n_y_dc_max_idx = n_cnt_i;

                }
            }

            //red
            n_y_ac = ( an_y[ an_ir_valley_locs[ n_cnt_k + 1 ] ] - an_y[ an_ir_valley_locs[ n_cnt_k ] ] ) * ( n_y_dc_max_idx - an_ir_valley_locs[ n_cnt_k ] );
            n_y_ac =  an_y[an_ir_valley_locs[ n_cnt_k ] ] + n_y_ac / ( an_ir_valley_locs[ n_cnt_k + 1 ] - an_ir_valley_locs[ n_cnt_k ] );
            // subracting linear DC compoenents from raw
            n_y_ac =  an_y[ n_y_dc_max_idx ] - n_y_ac;
            // ir
            n_x_ac = ( an_x[ an_ir_valley_locs[ n_cnt_k + 1 ] ] - an_x[ an_ir_valley_locs[ n_cnt_k ] ] ) * ( n_x_dc_max_idx - an_ir_valley_locs[ n_cnt_k ] );
            // subracting linear DC compoenents from raw
            n_x_ac =  an_x[ an_ir_valley_locs[ n_cnt_k ] ] + n_x_ac / ( an_ir_valley_locs[ n_cnt_k + 1 ] - an_ir_valley_locs[ n_cnt_k ] );
            n_x_ac =  an_x[ n_y_dc_max_idx ] - n_x_ac;
            //prepare X100 to preserve floating value
            n_nume =( n_y_ac * n_x_dc_max ) >> 7;
            n_denom = ( n_x_ac * n_y_dc_max ) >> 7;

            if ( ( n_denom > 0 )  && ( n_i_ratio_count < 5 ) && ( n_nume != 0 ) )
            {
                an_ratio[ n_i_ratio_count ] = ( n_nume * 100 ) / n_denom;
                n_i_ratio_count++;
            }
        }
    }

    // choose median value since PPG signal may varies from beat to beat
    dev_sort_ascend( an_ratio, n_i_ratio_count );
    n_middle_idx = n_i_ratio_count / 2;

    if ( n_middle_idx > 1 )
    {
        // use median
        n_ratio_average = ( an_ratio[ n_middle_idx - 1 ] + an_ratio[ n_middle_idx ] ) / 2;
    }
    else
    {
        n_ratio_average = an_ratio[ n_middle_idx ];
    }

    if ( ( n_ratio_average > 2 ) && ( n_ratio_average < 184 ) )
    {
        n_spo2_calc = uch_spo2_table[ n_ratio_average ];
        *pn_spo2 = (uint8)n_spo2_calc;
        error_flag = OXIMETER5_OK;
    }
    else
    {
        *pn_spo2 = OXIMETER5_PN_SPO2_ERROR_DATA;
        error_flag  = OXIMETER5_ERROR;
    }

    return error_flag;
}

oximeter5_return_value_t oximeter5_get_heart_rate ( uint32 *pun_ir_buffer, sint32 n_ir_buffer_length, uint32 *pun_red_buffer, sint32 *pn_heart_rate )
{
    uint32 un_ir_mean;
    sint32 n_th1, n_npks;
    sint32 an_ir_valley_locs[ 15 ];
    sint32 n_peak_interval_sum;
    sint32 an_x[ BUFFER_SIZE ];
     oximeter5_return_value_t error_flag;

    // calculates DC mean and subtract DC from ir
    un_ir_mean = 0;
    for ( sint32 n_cnt_k = 0; n_cnt_k < n_ir_buffer_length; n_cnt_k++ )
    {
        un_ir_mean += pun_ir_buffer[ n_cnt_k ];
    }

    un_ir_mean = un_ir_mean / n_ir_buffer_length;

    // remove DC and invert signal so that we can use peak detector as valley detector
    for ( sint32 n_cnt_k = 0; n_cnt_k < n_ir_buffer_length; n_cnt_k++ )
    {
        an_x[ n_cnt_k ] = -1 * ( pun_ir_buffer[ n_cnt_k ] - un_ir_mean );
    }

    // 4 pt Moving Average
    for( sint32 n_cnt_k = 0; n_cnt_k < BUFFER_SIZE - MA4_SIZE; n_cnt_k++ )
    {
        an_x[ n_cnt_k ]=( an_x[ n_cnt_k ] + an_x[ n_cnt_k + 1 ] + an_x[ n_cnt_k + 2 ] + an_x[ n_cnt_k + 3 ] ) / 4;
    }

    // calculate threshold
    n_th1 = 0;
    for ( sint32 n_cnt_k = 0; n_cnt_k < BUFFER_SIZE; n_cnt_k++ )
    {
        n_th1 +=  an_x[ n_cnt_k ];
    }

    n_th1 = n_th1 / BUFFER_SIZE;

    if ( n_th1 < 30 )
    {
        n_th1 = 30; // min allowed
    }

    if( n_th1 > 60 )
    {
        n_th1 = 60; // max allowed
    }

    for ( sint32 n_cnt_k = 0; n_cnt_k < 15; n_cnt_k++ )
    {
        an_ir_valley_locs[ n_cnt_k ] = 0;
    }

    // since we flipped signal, we use peak detector as valley detector
    dev_find_peaks( an_ir_valley_locs, &n_npks, an_x, BUFFER_SIZE, n_th1, 4, 15 );//peak_height, peak_distance, max_num_peaks

    n_peak_interval_sum = 0;

    if ( n_npks >= 2 )
    {
        for ( sint32 n_cnt_k = 1; n_cnt_k < n_npks; n_cnt_k++ )
        {
            n_peak_interval_sum += ( an_ir_valley_locs[ n_cnt_k ] -an_ir_valley_locs[ n_cnt_k - 1 ] );
        }

        n_peak_interval_sum = n_peak_interval_sum / ( n_npks - 1 );
        *pn_heart_rate = ( sint32 ) ( ( SAMPLING_FREQUENCY * 60 ) / n_peak_interval_sum );
        error_flag  = OXIMETER5_OK;
    }
    else
    {
        *pn_heart_rate = OXIMETER5_HEART_RATE_ERROR_DATA; // unable to calculate because # of peaks are too small
        error_flag  = OXIMETER5_ERROR;
    }

  return error_flag;

}

static void dev_peaks_above_min_height ( sint32 *pn_locs, sint32 *n_npks,  sint32  *pn_x, uint8 n_size, sint32 n_min_height )
{
    uint8 n_width;
    uint8 n_cnt = 1;

    *n_npks = 0;

    while ( n_cnt < ( n_size - 1 ) )
    {
        if ( pn_x[ n_cnt ] > n_min_height && pn_x[ n_cnt ] > pn_x[ n_cnt - 1 ] )
        {
            n_width = 1;

            while ( n_cnt + n_width < n_size && pn_x[ n_cnt ] == pn_x[ n_cnt + n_width ] )
            {
                n_width++;
            }

            if ( pn_x[ n_cnt ] > pn_x[ n_cnt + n_width ] && ( *n_npks ) < 15 )
            {
                pn_locs[( *n_npks )++ ] = n_cnt;
                n_cnt += n_width + 1;
            }
            else
            {
                n_cnt += n_width;
            }
        }
        else
        {
            n_cnt++;
        }
    }
}


static void dev_sort_indices_descend ( sint32 *pn_x, sint32 *pn_indx, sint32 n_size )
{
    sint32 n_temp;

    for ( sint32 n_cnt_i = 1; n_cnt_i < n_size; n_cnt_i++ )
    {
        n_temp = pn_indx[ n_cnt_i ];

        sint32 n_cnt_j;
        for ( n_cnt_j = n_cnt_i; n_cnt_j > 0 && pn_x[ n_temp ] > pn_x[ pn_indx[ n_cnt_j - 1 ] ]; n_cnt_j-- )
        {
            pn_indx[ n_cnt_j ] = pn_indx[ n_cnt_j - 1 ];
        }

        pn_indx[ n_cnt_j ] = n_temp;
    }
}

static void dev_sort_ascend ( sint32  *pn_x, sint32 n_size )
{
    sint32 n_temp;

    for ( sint32 n_cnt_i = 1; n_cnt_i < n_size; n_cnt_i++ )
    {
        n_temp = pn_x[ n_cnt_i ];

        sint32 n_cnt_j;
        for ( n_cnt_j = n_cnt_i; n_cnt_j > 0 && n_temp < pn_x[ n_cnt_j - 1 ]; n_cnt_j-- )
        {
            pn_x[ n_cnt_j ] = pn_x[ n_cnt_j - 1 ];
        }

        pn_x[ n_cnt_j ] = n_temp;
    }
}

static void dev_remove_close_peaks ( sint32 *pn_locs, sint32 *pn_npks, sint32 *pn_x, sint32 n_min_distance )
{
    sint32 n_old_npks, n_dist;

    dev_sort_indices_descend( pn_x, pn_locs, *pn_npks );

    for ( sint32 n_cnt_i = -1; n_cnt_i < *pn_npks; n_cnt_i++ )
    {
        n_old_npks = *pn_npks;
        *pn_npks = n_cnt_i + 1;

        for ( sint32 n_cnt_j = n_cnt_i + 1; n_cnt_j < n_old_npks; n_cnt_j++ )
        {
            n_dist =  pn_locs[ n_cnt_j ] - ( n_cnt_i == -1 ? -1 : pn_locs[ n_cnt_i ] );

            if ( n_dist > n_min_distance || n_dist < -n_min_distance )
            {
                pn_locs[ (*pn_npks)++ ] = pn_locs[ n_cnt_j ];
            }
        }
    }

    dev_sort_ascend( pn_locs, *pn_npks );
}

static void dev_find_peaks ( sint32 *pn_locs, sint32 *n_npks,  sint32  *pn_x, uint8 n_size, sint32 n_min_height, sint32 n_min_distance, sint32 n_max_num )
{
    dev_peaks_above_min_height( pn_locs, n_npks, pn_x, n_size, n_min_height );
    dev_remove_close_peaks( pn_locs, n_npks, pn_x, n_min_distance );
    if ( *n_npks > n_max_num )
    {
        *n_npks = n_max_num;
    }
}

static void Delay_10ms ( void ){
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 10));
}

static void Delay_1sec ( void ){
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 1000));
}

// ------------------------------------------------------------------------- END
