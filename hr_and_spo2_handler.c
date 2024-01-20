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

#include "hr_and_spo2_handler.h"

#include <Bsp.h>                      //Board support functions (for the waitTime function)

// todo doc/comments

static uint8 spo2_value;
static sint32 heart_rate_value;
static IfxCpu_mutexLock resource_lock;

interface_return_value_t prepare_oximeter5_hardware(oximeter5_t *oximeter5, uint32 *ir_buffer, uint32 *red_buffer){
    oximeter5_init(oximeter5);
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 100));

    if(oximeter5_default_cfg(oximeter5) == OXIMETER5_ERROR)
        return SENSOR_ERROR;
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 100));

    for(uint8 n_cnt = 0; n_cnt < BUFFER_SIZE; n_cnt++){
        while(oximeter5_check_interrupt(oximeter5) == OXIMETER5_INTERRUPT_ACTIVE);
        if(oximeter5_read_sensor_data(oximeter5, &red_buffer[n_cnt], &ir_buffer[n_cnt]) == OXIMETER5_ERROR)
            return SENSOR_ERROR;
    }

    uint8 spo2_test = 255;
    if(oximeter5_get_oxygen_saturation(&ir_buffer[0], BUFFER_SIZE, &red_buffer[0], &spo2_test) == OXIMETER5_ERROR)
        return CALCULATION_ERROR;
    return SUCCESS;
}


interface_return_value_t read_and_calculate_values(oximeter5_t *oximeter5, uint32 *ir_buffer, uint32 *red_buffer){
    for(uint8 n_cnt = SAMPLING_FREQUENCY; n_cnt < BUFFER_SIZE; n_cnt++){
        red_buffer[n_cnt - SAMPLING_FREQUENCY] = red_buffer[n_cnt];
        ir_buffer[n_cnt - SAMPLING_FREQUENCY] = ir_buffer[n_cnt];
    }

    for(uint8 n_cnt = (BUFFER_SIZE - SAMPLING_FREQUENCY); n_cnt < BUFFER_SIZE; n_cnt++){
        while(oximeter5_check_interrupt(oximeter5) == OXIMETER5_INTERRUPT_ACTIVE);
        if(oximeter5_read_sensor_data(oximeter5, &red_buffer[n_cnt], &ir_buffer[n_cnt]) == OXIMETER5_ERROR)
            return SENSOR_ERROR;
    }

    uint8 spo2_temp = 255;
    sint32 hr_temp = -999;
    if(oximeter5_get_oxygen_saturation(&ir_buffer[0], BUFFER_SIZE, &red_buffer[0], &spo2_temp) == OXIMETER5_ERROR)
        return CALCULATION_ERROR;
    if(oximeter5_get_heart_rate(&ir_buffer[0], BUFFER_SIZE, &red_buffer[0], &hr_temp) == OXIMETER5_ERROR)
        return CALCULATION_ERROR;

    boolean mutex_flag = IfxCpu_acquireMutex(&resource_lock);
    if (!mutex_flag){
        return SAVE_ERROR;
    }
    spo2_value = spo2_temp;
    heart_rate_value = hr_temp;
    IfxCpu_releaseMutex(&resource_lock);
    return SUCCESS;
}

interface_return_value_t get_values(uint8 *spo2, sint32 *heart_rate){
    boolean mutex_flag = IfxCpu_acquireMutex(&resource_lock);
    if (!mutex_flag){
        return LOAD_ERROR;
    }
    *spo2 = spo2_value;
    *heart_rate = heart_rate_value;
    IfxCpu_releaseMutex(&resource_lock);
    return SUCCESS;
}
