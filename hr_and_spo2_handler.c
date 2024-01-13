/*
 * hr_and_spo2_handler.c
 *
 *  Created on: 11.01.2024
 *      Author: Andreas Reichenauer
 */

/*!
 * @file oximeter5_click.h
 * @brief This file implements interfacing with the Oximeter 5 Click Driver and saving the obtained values in a shared memory.
 */

#include <Bsp.h>                      //Board support functions (for the waitTime function)

#include "hr_and_spo2_handler.h"

static oximeter5_t oximeter5;
static uint32 aun_ir_buffer[BUFFER_SIZE];
static uint32 aun_red_buffer[BUFFER_SIZE];
static uint8 n_spo2;
static sint32 n_hr;

oximeter5_return_value_t prepare_oximeter5_hardware(){
    oximeter5_init(&oximeter5);
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 100));

    if(oximeter5_default_cfg(&oximeter5) == OXIMETER5_ERROR)
        return OXIMETER5_ERROR;
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 100));

    for (uint8 n_cnt = 0; n_cnt < BUFFER_SIZE; n_cnt++)
    {
        while ( oximeter5_check_interrupt( &oximeter5 ) == OXIMETER5_INTERRUPT_ACTIVE );

        if(oximeter5_read_sensor_data( &oximeter5, &aun_red_buffer[ n_cnt ], &aun_ir_buffer[ n_cnt ] ) == OXIMETER5_ERROR)
            return OXIMETER5_ERROR;
    }

    if(oximeter5_get_oxygen_saturation( &aun_ir_buffer[ 0 ], BUFFER_SIZE, &aun_red_buffer[ 0 ], &n_spo2 ) == OXIMETER5_ERROR)
        return OXIMETER5_ERROR;
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 100));
    return OXIMETER5_OK;
}


oximeter5_return_value_t read_and_save_values(){
    for ( uint8 n_cnt = SAMPLING_FREQUENCY; n_cnt < BUFFER_SIZE; n_cnt++ )
    {
        aun_red_buffer[ n_cnt - SAMPLING_FREQUENCY] = aun_red_buffer[ n_cnt ];
        aun_ir_buffer[ n_cnt - SAMPLING_FREQUENCY] = aun_ir_buffer[ n_cnt ];
    }

    for ( uint8 n_cnt = (BUFFER_SIZE - SAMPLING_FREQUENCY); n_cnt < BUFFER_SIZE; n_cnt++ )
    {
        while ( oximeter5_check_interrupt( &oximeter5 ) == OXIMETER5_INTERRUPT_ACTIVE );

        if(oximeter5_read_sensor_data( &oximeter5, &aun_red_buffer[ n_cnt ], &aun_ir_buffer[ n_cnt ] ) == OXIMETER5_ERROR)
            return OXIMETER5_ERROR;
    }
    if(oximeter5_get_heart_rate(&aun_ir_buffer[ 0 ], BUFFER_SIZE, &aun_red_buffer[ 0 ], &n_hr) == OXIMETER5_ERROR)
        return OXIMETER5_ERROR;
    if(oximeter5_get_oxygen_saturation( &aun_ir_buffer[ 0 ], BUFFER_SIZE, &aun_red_buffer[ 0 ], &n_spo2 ) == OXIMETER5_ERROR)
        return OXIMETER5_ERROR;
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 100));
    return OXIMETER5_OK;
}
