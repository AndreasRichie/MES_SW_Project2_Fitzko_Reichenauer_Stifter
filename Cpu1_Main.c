/**********************************************************************************************************************
 * \file Cpu1_Main.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 * 
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of 
 * business you are acting and (ii) Infineon Technologies AG or its licensees. If and as long as no such terms of use
 * are agreed, use of this file is subject to following:
 * 
 * Boost Software License - Version 1.0 - August 17th, 2003
 * 
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and 
 * accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 * and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 * 
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 * and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all 
 * derivative works of the Software, unless such copies or derivative works are solely in the form of 
 * machine-executable object code generated by a source language processor.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE 
 * COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN 
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 * IN THE SOFTWARE.
 *********************************************************************************************************************/
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include "hr_and_spo2_handler.h"
#include "sensor_timer.h"

extern IfxCpu_syncEvent g_cpuSyncEvent;

void handle_error(const interface_return_value_t error){
    // if calculation error or save error occurred still continue with next reading
    if(error != SENSOR_ERROR)
        return;
    // stop reading and start delay
    stop_read_timer();
    start_error_timer();
}

void handle_read(void){
    // check return value of calculation
    handle_error(read_and_calculate_values());
}

void handle_restart(void){
    // stop delay timer and restart read timer
    stop_error_timer();
    start_read_timer();
}

int core1_main(void)
{
    IfxCpu_enableInterrupts();
    
    /* !!WATCHDOG1 IS DISABLED HERE!!
     * Enable the watchdog and service it periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    
    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);

    // prepare oximeter 5 hardware for usage
    interface_return_value_t oximeter_error = prepare_oximeter5_hardware();

    // critical error if hardware can not be prepared, do not continue further
    if(oximeter_error == SENSOR_ERROR)
        return -1;

    // initialize read and error timers
    init_error_timer((interrupt_fptr_t)handle_restart);
    init_read_timer((interrupt_fptr_t)handle_read);
    // start value reading
    start_read_timer();

    while(1)
    {
    }
    return (1);
}
