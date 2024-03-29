/*
 * UART.c
 *
 *  Created on: 2 Nov 2022
 *  Author: Technikum Wien
 *  Modified: Andreas Reichenauer and Gabriel Fitzko
 *
 *  A working UART implementation with custom wrapper functions of the Asclin UART functions
 *  If more functionality is needed, make wrapper functions of the original UART functions.
 *  The original functions can be found in Asclin/Asc/IfxAsclin_Asc.h
 *
 */

#include <UART.h>
#include <Asclin/Asc/IfxAsclin_Asc.h>
#include "IfxCpu_Irq.h"
#include <stdio.h>
#include <string.h>

/*************************************************************************************************************/
/*------------------------------------------------------Macros-----------------------------------------------*/
/*************************************************************************************************************/
#define SERIAL_BAUDRATE         115200                              //Baud rate in bit/s

#define SERIAL_PIN_RX_CON_1     IfxAsclin3_RXD_P32_2_IN             // RX pin of the board
#define SERIAL_PIN_TX_CON_1     IfxAsclin3_TX_P15_7_OUT             // TX pin of the board

#define ASC_TX_BUFFER_SIZE      64                                  // Definition of the buffer size
#define ASC_RX_BUFFER_SIZE      64                                  // Definition of the buffer size

#define INTPRIO_ASCLIN3_TX 1                                        // Interrupt priority for UART transfer
#define INTPRIO_ASCLIN3_RX 2                                        // Interrupt priority for UART receive
#define INTPRIO_ASCLIN3_ER 3                                        // Interrupt priority for UART Error

#define SIZE_DEVICE_ID_STRING   23                                  // Size of string necessary for serial id
#define SIZE_VALUES_STRING      45                                  // Size of string reserved for sending values

/*************************************************************************************************************/
/*-------------------------------------------------Global variables------------------------------------------*/
/*************************************************************************************************************/
IfxAsclin_Asc asc;                                                  // Declaration of the ASC handle
uint8 ascTxBuffer[ASC_TX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];       // Declaration of the FIFOs parameters
uint8 ascRxBuffer[ASC_RX_BUFFER_SIZE + sizeof(Ifx_Fifo) + 8];       // Declaration of the FIFOs parameters

static char value_string[SIZE_VALUES_STRING] = {0};                 // Buffer for values string
static char timestamp_buf[SIZE_VALUES_STRING] = {0};                 // Buffer for values string

/*********************************************************************************************************************/
/*---------------------------------------------Function Implementations----------------------------------------------*/
/*********************************************************************************************************************/
IFX_INTERRUPT(asclin3_Tx_ISR, 2, INTPRIO_ASCLIN3_TX);               // Adding the Interrupt Service Routine
IFX_INTERRUPT(asclin3_Rx_ISR, 2, INTPRIO_ASCLIN3_RX);               // Adding the Interrupt Service Routine
IFX_INTERRUPT(asclin3_Er_ISR, 2, INTPRIO_ASCLIN3_ER);               // Adding the Interrupt Service Routine


void asclin3_Tx_ISR(void) {
    IfxAsclin_Asc_isrTransmit(&asc);
}

void asclin3_Rx_ISR(void) {
    IfxAsclin_Asc_isrReceive(&asc);
}

void asclin3_Er_ISR(void) {
    IfxAsclin_Asc_isrError(&asc);
}

void initUART() {

    /* Initialize an instance of IfxAsclin_Asc_Config with default values */
        IfxAsclin_Asc_Config ascConfig;
        IfxAsclin_Asc_initModuleConfig(&ascConfig, SERIAL_PIN_TX_CON_1.module);

        /* Set the desired baud rate */
        ascConfig.baudrate.prescaler = 1;
        ascConfig.baudrate.baudrate = SERIAL_BAUDRATE;
        ascConfig.baudrate.oversampling = IfxAsclin_OversamplingFactor_16;

        /* ISR priorities and interrupt target */
        ascConfig.interrupt.txPriority = INTPRIO_ASCLIN3_TX;
        ascConfig.interrupt.rxPriority = INTPRIO_ASCLIN3_RX;
        ascConfig.interrupt.erPriority = INTPRIO_ASCLIN3_ER;
        ascConfig.interrupt.typeOfService = IfxCpu_Irq_getTos(IfxCpu_getCoreIndex());
        //ascConfig.interrupt.typeOfService = (IfxSrc_Tos)IfxCpu_getCoreId();


        /* FIFO configuration */
        ascConfig.txBuffer = &ascTxBuffer;
        ascConfig.txBufferSize = ASC_TX_BUFFER_SIZE;
        ascConfig.rxBuffer = &ascRxBuffer;
        ascConfig.rxBufferSize = ASC_RX_BUFFER_SIZE;

        /* Port pins configuration */
        const IfxAsclin_Asc_Pins pins = {
            NULL_PTR,         IfxPort_InputMode_pullUp,             // CTS pin not used
            &SERIAL_PIN_RX_CON_1,   IfxPort_InputMode_pullUp,       // RX pin not used
            NULL_PTR,         IfxPort_OutputMode_pushPull,          // RTS pin not used
            &SERIAL_PIN_TX_CON_1,   IfxPort_OutputMode_pushPull,    // TX pin
            IfxPort_PadDriver_cmosAutomotiveSpeed1
        };
        ascConfig.pins = &pins;

        IfxAsclin_Asc_initModule(&asc, &ascConfig);

}

//Sends one byte via UART
void uart_blockingWrite(uint8 byte) {
    IfxAsclin_Asc_blockingWrite(&asc, byte);
}


//Sends a string (message) via UART
void uart_sendMessage(uint8 *data, Ifx_SizeT size) {
    IfxAsclin_Asc_write(&asc, data, &size, TIME_INFINITE);
}

/*
 * This function receives two values:
 * hr (Heartrate) and SpO2 (blood oxygen saturation)
 * It converts these two values with the string.h function "snprintf" into a string and
 * then sends it via UART
 */

void send_values(const uint8 hr, const sint32 spo2){

    snprintf(value_string, sizeof(value_string), "%dBPM, %d%%SpO2,\n", hr, spo2);
    // Send converted string via UART
    uart_sendMessage((uint8*)value_string, strlen(value_string));
}

/*
 * This function receives three values:
 * secs (seconds), mins (minutes) and hours (hours)
 * These values are the passed time and they get calculated in STM_Interrupt.c
 * It converts these two values with the string.h function "snprintf" into a string and
 * then sends it via UART
 *
 */
void send_timestamp (const uint8 secs, const uint8 mins, const uint8 hours){

    snprintf(timestamp_buf, sizeof(timestamp_buf), "[%02dh:%02dm:%02ds] ", hours, mins, secs);
    uart_sendMessage((uint8*)timestamp_buf, strlen(timestamp_buf));
}



