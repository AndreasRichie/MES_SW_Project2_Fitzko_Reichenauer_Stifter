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
#include <UART.h>

#include <timer.h>



IfxCpu_syncEvent g_cpuSyncEvent = 0;




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

    initUART();                                             // Initialize UART ! need to set interrupts in uart.c to 2 as well!

//    init_i2c();                                             // Initialize I2C
//    do_soft_reset();                                        // Perform soft reset of sensor
//    read_and_print_serial_id();                             // Read and send serial id of sensor
//    init_timer((interrupt_fptr_t)read_and_send_values);     // Initialize and start timer





    while(1) {

        //scream
    }

    return (1);
}
