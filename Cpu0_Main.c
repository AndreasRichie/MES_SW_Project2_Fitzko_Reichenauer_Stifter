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
#include "__c8x8r_driver.h"
#include <Bsp.h>

IFX_INTERRUPT(qspi0TxISR, 0, IFX_INTPRIO_QSPI0_TX)
{
    IfxQspi_SpiMaster_isrTransmit(&spi);
}


IFX_INTERRUPT(qspi0RxISR, 0, IFX_INTPRIO_QSPI0_RX)
{
   IfxQspi_SpiMaster_isrReceive(&spi);
}

IFX_INTERRUPT(qspi0ErISR, 0, IFX_INTPRIO_QSPI0_ER)
{
   IfxQspi_SpiMaster_isrError(&spi);
}

IfxCpu_syncEvent g_cpuSyncEvent = 0;

int core0_main(void) {

    IfxCpu_enableInterrupts();
    
    uint8 image_big[8] = {0};                   // container for image
    uint8 image_small[8] = {0};                 // container for image
    struct display_data data = {0};             // storage for received data
    uint32 timings = 0;                         // calculation for timings
    uint8 counter = 0;                          // timer for gathering sensor Data

    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
    
    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);
    

    c8x8r_init();                                           // Initialize the display

    while(1) {
        if(counter > 5){
            get_globals(&data);                             // Get the data from memory location
            timings = c8x8r_getHeartFrequenz(data.bpm);     // Calculate Timings
            change_images(&data, image_big, image_small);   // Change image according to SPO2 and BPM
            counter = 0;
        }

        c8x8r_displayRefresh();                                                                     // Refreshes display before new image creation
        c8x8r_displayImage(image_big);                                                              // Display big image
        waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, (uint16)(timings >> 16)));      // Wait time according to systolic timing

        c8x8r_displayRefresh();
        c8x8r_displayImage(image_small);                                                            // Displays the small image
        waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, (uint16)timings));              // Wait time according to diastolic timing

        counter ++;
    }

    return (1);
}
