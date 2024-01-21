/**********************************************************************************************************************
 * \file STM_Interrupt.c
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

/*********************************************************************************************************************/
/*-----------------------------------------------------Includes------------------------------------------------------*/
/*********************************************************************************************************************/
#include "STM_Interrupt.h"
#include "Bsp.h"
#include "IfxPort.h"
#include "IfxStm.h"
#include <Asclin/Asc/IfxAsclin_Asc.h> //Needed for UART
#include <UART.h>
#include "Ifx_Types.h"
#include <string.h>
#include "hr_and_spo2_handler.h"

/*********************************************************************************************************************/
/*------------------------------------------------------Macros-------------------------------------------------------*/
/*********************************************************************************************************************/
#define ISR_PRIORITY_STM        40                              /* Priority for interrupt ISR                       */
#define TIMER_INT_TIME          1000                             /* Time between interrupts in ms                    */

#define STM                     &MODULE_STM0                    /* STM0 is used in this example                     */

/*********************************************************************************************************************/
/*-------------------------------------------------Global variables--------------------------------------------------*/
/*********************************************************************************************************************/
IfxStm_CompareConfig g_STMConf;                                 /* STM configuration structure                      */
Ifx_TickTime g_ticksFor1s;                                   /* Variable to store the number of ticks to wait    */
boolean timer_flag = FALSE;

uint8    seconds = 0;
uint8    minutes = 0;
uint8    hours = 0;

/*********************************************************************************************************************/
/*------------------------------------------------Function Prototypes------------------------------------------------*/
/*********************************************************************************************************************/
static void generate_timestamp(void);
void initSTM(void);


/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
/* Macro to define Interrupt Service Routine.
 * This macro makes following definitions:
 * 1) Define linker section as .intvec_tc<vector number>_<interrupt priority>.
 * 2) define compiler specific attribute for the interrupt functions.
 * 3) define the Interrupt service routine as ISR function.
 *
 * IFX_INTERRUPT(isr, vectabNum, priority)
 *  - isr: Name of the ISR function.
 *  - vectabNum: Vector table number.
 *  - priority: Interrupt priority. Refer Usage of Interrupt Macro for more details.
 */
IFX_INTERRUPT(isrSTM, 0, ISR_PRIORITY_STM);

void isrSTM(void)
{

    uint8 spo2_value = 0;
    sint32 heart_rate_value = 0;

    /* Update the compare register value that will trigger the next interrupt and toggle the LED */
    IfxStm_increaseCompare(STM, g_STMConf.comparator, g_ticksFor1s);

    interface_return_value_t oximeter_error = get_values(&spo2_value, &heart_rate_value);


    if(oximeter_error == SUCCESS){
        // just to have all values available in debugger
        uint8 test_spo2 = spo2_value;
        sint32 test_hr = heart_rate_value;

        generate_timestamp();
        send_values(heart_rate_value, spo2_value);
    }




}

/* Function to initialize the STM */
void initSTM(void)
{
    IfxStm_initCompareConfig(&g_STMConf);           /* Initialize the configuration structure with default values   */

    g_STMConf.triggerPriority = ISR_PRIORITY_STM;   /* Set the priority of the interrupt                            */
    g_STMConf.typeOfService = IfxSrc_Tos_cpu0;      /* Set the service provider for the interrupts                  */
    g_STMConf.ticks = g_ticksFor1s;                 /* Set the number of ticks after which the timer triggers an
                                                     * interrupt for the first time                                 */
    IfxStm_initCompare(STM, &g_STMConf);            /* Initialize the STM with the user configuration               */
}

/* Function to initialize all the peripherals and variables used */
void initCommTimer(void)
{
    /* Initialize time constant */
    g_ticksFor1s = IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, TIMER_INT_TIME);

    initSTM();                                      /* Configure the STM module                                     */
}




static void generate_timestamp(void){

  //another trigger means one second has passed
  seconds++;
  if (seconds >= 60){
    minutes ++;
    seconds = seconds - 60;
  }
  if (minutes >= 60){
    hours ++;
    minutes = minutes - 60;
  }

  send_timestamp(seconds, minutes, hours);

}





