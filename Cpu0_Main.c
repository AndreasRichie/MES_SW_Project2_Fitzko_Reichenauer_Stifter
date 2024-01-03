/**********************************************************************************************************************
 * \file Cpu0_Main.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 * 
 *  Created on: 2 Nov 2022
 *  Author: Technikum Wien
 * 
 * 
 *********************************************************************************************************************/
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"
#include <Asclin/Asc/IfxAsclin_Asc.h> //Needed for UART
#include <timer.h>
#include <UART.h>
#include <i2c_sensor.h>

IfxCpu_syncEvent g_cpuSyncEvent = 0;

void read_and_send_values(void)
{
    float temperature = 0.f;                                // Variable to store the temperature value in
    float humidity = 0.f;                                   // Variable to store the humidity value in
    read_and_calculate_values(&temperature, &humidity);     // Read values from sensor
    send_values(temperature, humidity);                     // Send read values via UART
}


void read_and_print_serial_id(){
    uint32 serial_id = read_serial_id();    // Read serial id from sensor
    send_serial_id(serial_id);              // Send serial id via UART
}


int core0_main(void) {

    IfxCpu_enableInterrupts();
    
    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    
    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);
    
    initUART();                                             // Initialize UART
    init_i2c();                                             // Initialize I2C
    do_soft_reset();                                        // Perform soft reset of sensor
    read_and_print_serial_id();                             // Read and send serial id of sensor
    init_timer((interrupt_fptr_t)read_and_send_values);     // Initialize and start timer

    while(1) {
    }

    return (1);
}
