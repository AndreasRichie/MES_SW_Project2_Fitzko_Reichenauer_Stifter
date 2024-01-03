/*
 * timer.c
 *
 *  Created on: 10.12.2023
 *      Author: Andreas Reichenauer
 */

#include "timer.h"
#include <IfxGtm_Tom_Timer.h>

/*************************************************************************************************************/
/*------------------------------------------------------Macros-----------------------------------------------*/
/*************************************************************************************************************/
#define ISR_PRIORITY_TOM        4       // Timer interrupt priority (must be less important than UART or Tx does not work correctly)
#define TOM_FREQ                2.f     // Timer frequency in Hz --> 2Hz for every 500ms send

/*************************************************************************************************************/
/*-------------------------------------------------Global variables------------------------------------------*/
/*************************************************************************************************************/
static IfxGtm_Tom_Timer g_timerDriver;              // Global variable for timer struct
static interrupt_fptr_t interrupt_function = NULL;  // Function used for interrupt handling

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
IFX_INTERRUPT(interruptGtmTom, 0, ISR_PRIORITY_TOM);    // Adding the Interrupt Service Routine


void interruptGtmTom(void)
{
    IfxGtm_Tom_Timer_acknowledgeTimerIrq(&g_timerDriver);               // Clear the timer event

    if(interrupt_function == NULL) return;                              // If defined use the interrupt function for further handling
    interrupt_function();
}


void init_timer(interrupt_fptr_t interrupt_function_){
    IfxGtm_enable(&MODULE_GTM);                                         // Enable GTM

    IfxGtm_Tom_Timer_Config timerConfig;                                // Timer configuration
    IfxGtm_Tom_Timer_initConfig(&timerConfig, &MODULE_GTM);             // Initialize timer configuration

    timerConfig.base.frequency       = TOM_FREQ;                        // Set timer frequency
    timerConfig.base.isrPriority     = ISR_PRIORITY_TOM;                // Set interrupt priority
    timerConfig.base.isrProvider     = IfxSrc_Tos_cpu0;                 // Set interrupt provider
    timerConfig.tom                  = IfxGtm_Tom_1;                    // Define the timer used
    timerConfig.timerChannel         = IfxGtm_Tom_Ch_0;                 // Define the channel used
    timerConfig.clock                = IfxGtm_Tom_Ch_ClkSrc_cmuFxclk3;  // Define the CMU clock used

    IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_FXCLK);       // Enable the CMU clock
    IfxGtm_Tom_Timer_init(&g_timerDriver, &timerConfig);                // Initialize the TOM

    IfxGtm_Tom_Timer_run(&g_timerDriver);                               // Start the TOM

    interrupt_function = interrupt_function_;                           // Set interrupt handling function
}

