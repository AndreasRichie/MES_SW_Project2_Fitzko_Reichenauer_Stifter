/*
 * sensor_timer.h
 *
 *  Created on: 20.01.2024
 *      Author: Andreas Reichenauer
 */

#include <IfxGtm_Tom_Timer.h>
#include <sensor_timer.h>

/*************************************************************************************************************/
/*------------------------------------------------------Macros-----------------------------------------------*/
/*************************************************************************************************************/
#define ISR_PRIORITY_TOM_READ         5        // Timer interrupt priority for read timer
#define ISR_PRIORITY_TOM_ERROR        4        // Timer interrupt priority for error timer
#define TOM_FREQ_READ                 10.f     // Timer frequency in Hz --> 10Hz for every 100ms read
#define TOM_FREQ_ERROR                0.2f     // Timer frequency in Hz --> 0.2Hz for 5s wait after error

/*************************************************************************************************************/
/*-------------------------------------------------Global variables------------------------------------------*/
/*************************************************************************************************************/
static IfxGtm_Tom_Timer g_timerDriver_read;               // Global variable for read timer struct
static interrupt_fptr_t interrupt_function_read = NULL;   // Function used for read interrupt handling
static IfxGtm_Tom_Timer g_timerDriver_error;              // Global variable for error timer struct
static interrupt_fptr_t interrupt_function_error = NULL;  // Function used for error interrupt handling

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
IFX_INTERRUPT(interruptReadTimer, 1, ISR_PRIORITY_TOM_READ);      // Adding the read Interrupt Service Routine
IFX_INTERRUPT(interruptErrorTimer, 1, ISR_PRIORITY_TOM_ERROR);    // Adding the error Interrupt Service Routine


void interruptReadTimer(void)
{
    IfxGtm_Tom_Timer_acknowledgeTimerIrq(&g_timerDriver_read);          // Clear the timer event

    if(interrupt_function_read == NULL) return;                         // If defined use the interrupt function for further handling
    interrupt_function_read();
}


void interruptErrorTimer(void)
{
    IfxGtm_Tom_Timer_acknowledgeTimerIrq(&g_timerDriver_error);         // Clear the timer event

    if(interrupt_function_error == NULL) return;                        // If defined use the interrupt function for further handling
    interrupt_function_error();
}


void init_read_timer(interrupt_fptr_t interrupt_function_){
    IfxGtm_enable(&MODULE_GTM);                                         // Enable GTM

    IfxGtm_Tom_Timer_Config timerConfig;                                // Timer configuration
    IfxGtm_Tom_Timer_initConfig(&timerConfig, &MODULE_GTM);             // Initialize timer configuration

    timerConfig.base.frequency       = TOM_FREQ_READ;                   // Set timer frequency
    timerConfig.base.isrPriority     = ISR_PRIORITY_TOM_READ;           // Set interrupt priority
    timerConfig.base.isrProvider     = IfxSrc_Tos_cpu1;                 // Set interrupt provider
    timerConfig.tom                  = IfxGtm_Tom_1;                    // Define the timer used
    timerConfig.timerChannel         = IfxGtm_Tom_Ch_0;                 // Define the channel used
    timerConfig.clock                = IfxGtm_Tom_Ch_ClkSrc_cmuFxclk3;  // Define the CMU clock used

    IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_FXCLK);       // Enable the CMU clock
    IfxGtm_Tom_Timer_init(&g_timerDriver_read, &timerConfig);           // Initialize the TOM

    interrupt_function_read = interrupt_function_;                      // Set interrupt handling function
}


void init_error_timer(interrupt_fptr_t interrupt_function_){
    IfxGtm_enable(&MODULE_GTM);                                         // Enable GTM

    IfxGtm_Tom_Timer_Config timerConfig;                                // Timer configuration
    IfxGtm_Tom_Timer_initConfig(&timerConfig, &MODULE_GTM);             // Initialize timer configuration

    timerConfig.base.frequency       = TOM_FREQ_ERROR;                  // Set timer frequency
    timerConfig.base.isrPriority     = ISR_PRIORITY_TOM_ERROR;          // Set interrupt priority
    timerConfig.base.isrProvider     = IfxSrc_Tos_cpu1;                 // Set interrupt provider
    timerConfig.tom                  = IfxGtm_Tom_2;                    // Define the timer used
    timerConfig.timerChannel         = IfxGtm_Tom_Ch_0;                 // Define the channel used
    timerConfig.clock                = IfxGtm_Tom_Ch_ClkSrc_cmuFxclk3;  // Define the CMU clock used

    IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_FXCLK);       // Enable the CMU clock
    IfxGtm_Tom_Timer_init(&g_timerDriver_error, &timerConfig);          // Initialize the TOM

    interrupt_function_error = interrupt_function_;                     // Set interrupt handling function
}


void start_read_timer(void){
    IfxGtm_Tom_Timer_run(&g_timerDriver_read);       // Start the TOM
}


void start_error_timer(void){
    IfxGtm_Tom_Timer_run(&g_timerDriver_error);      // Start the TOM
}


void stop_read_timer(void){
    IfxGtm_Tom_Timer_stop(&g_timerDriver_read);      // Stop the TOM
}


void stop_error_timer(void){
    IfxGtm_Tom_Timer_stop(&g_timerDriver_error);     // Stop the TOM
}
