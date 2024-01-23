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

// oximeter 5 click context object
static oximeter5_t oximeter5;
// buffers for IR and red brightness values
static uint32 ir_buffer[BUFFER_SIZE];
static uint32 red_buffer[BUFFER_SIZE];

// global values for shared memory/multicore communication
static uint8 spo2_value = 0;
static sint32 heart_rate_value = 0;
static IfxCpu_mutexLock resource_lock;

/**
 * @brief Delay execution for 10ms function.
 * @details This function delays the execution of the program for 10ms.
 */
static void delay_100ms ( void ){
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, 100));
}

interface_return_value_t prepare_oximeter5_hardware(void){
    // initialize I2C and Oximeter 5 and wait 100ms after
    oximeter5_init(&oximeter5);
    delay_100ms();

    // set default configuration to Oximeter 5 and wait 100ms after
    if(oximeter5_default_cfg(&oximeter5) == OXIMETER5_ERROR)
        return SENSOR_ERROR;
    delay_100ms();

    // read values if hardware interrupt occurs and fill up buffers
    for(uint8 n_cnt = 0; n_cnt < BUFFER_SIZE; n_cnt++){
        while(oximeter5_check_interrupt(&oximeter5) == OXIMETER5_INTERRUPT_ACTIVE);
        if(oximeter5_read_sensor_data(&oximeter5, &red_buffer[n_cnt], &ir_buffer[n_cnt]) == OXIMETER5_ERROR)
            return SENSOR_ERROR;
    }

    // spo2 test calculation with read values
    uint8 spo2_test = 255;
    if(oximeter5_get_oxygen_saturation(&ir_buffer[0], BUFFER_SIZE, &red_buffer[0], &spo2_test) == OXIMETER5_ERROR)
        return CALCULATION_ERROR;

    // no errors occurred
    return SUCCESS;
}


interface_return_value_t read_and_calculate_values(void){
    // shift values in buffers forwards by one sampling interval so end can be filled with next measurement
    for(uint8 n_cnt = SAMPLING_FREQUENCY; n_cnt < BUFFER_SIZE; n_cnt++){
        red_buffer[n_cnt - SAMPLING_FREQUENCY] = red_buffer[n_cnt];
        ir_buffer[n_cnt - SAMPLING_FREQUENCY] = ir_buffer[n_cnt];
    }

    // fill up end of buffers with next measurements when hardware interrupt occurs
    for(uint8 n_cnt = (BUFFER_SIZE - SAMPLING_FREQUENCY); n_cnt < BUFFER_SIZE; n_cnt++){
        while(oximeter5_check_interrupt(&oximeter5) == OXIMETER5_INTERRUPT_ACTIVE);
        if(oximeter5_read_sensor_data(&oximeter5, &red_buffer[n_cnt], &ir_buffer[n_cnt]) == OXIMETER5_ERROR)
            return SENSOR_ERROR;
    }

    uint8 spo2_temp = INVALID_SPO2;
    sint32 hr_temp = INVALID_HR;

    // calculate heart rate and spo2 values from buffers
    oximeter5_return_value_t calculation_error = oximeter5_get_oxygen_saturation(&ir_buffer[0], BUFFER_SIZE, &red_buffer[0], &spo2_temp);
    calculation_error |= oximeter5_get_heart_rate(&ir_buffer[0], BUFFER_SIZE, &red_buffer[0], &hr_temp);

    // check if mutex locked
    boolean mutex_flag = IfxCpu_acquireMutex(&resource_lock);

    // if locked return with save error
    if (!mutex_flag){
        return SAVE_ERROR;
    }

    // if not locked save calculated values into global variables, if there was a calculation error use invalid values
    spo2_value = (calculation_error == OXIMETER5_ERROR) ? INVALID_SPO2 : spo2_temp;
    heart_rate_value = (calculation_error == OXIMETER5_ERROR) ? INVALID_HR : hr_temp;

    // don't forget to release mutex after access
    IfxCpu_releaseMutex(&resource_lock);

    // calculation error occurred
    if(calculation_error == OXIMETER5_ERROR)
        return CALCULATION_ERROR;

    // no errors occurred
    return SUCCESS;
}

interface_return_value_t get_values(uint8 *spo2, sint32 *heart_rate){
    // check if mutex locked
    boolean mutex_flag = IfxCpu_acquireMutex(&resource_lock);

    // if locked return with load error
    if (!mutex_flag){
        return LOAD_ERROR;
    }

    // if not locked write last values to output parameters
    *spo2 = spo2_value;
    *heart_rate = heart_rate_value;

    // don't forget to release mutex after access
    IfxCpu_releaseMutex(&resource_lock);

    // no errors occurred
    return SUCCESS;
}
