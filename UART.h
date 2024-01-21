/*
 * UART.h
 *
 *  Created on: 2 Nov 2022
 *      Author: Technikum Wien
 *      Modified: Andreas Reichenauer
 */

#ifndef UART_H_
#define UART_H_

#include "Ifx_Types.h"

/***
 * @brief: functions to initialise the UART Interrupt routines
 * @params: None
 * @return: void
 */
void asclin3_Tx_ISR(void);
void asclin3_Rx_ISR(void);
void asclin3_Er_ISR(void);

/***
 * @brief: initialises the UART module
 * @params: None
 * @returns: void
 */
void initUART(void);

/***
 * @brief: a wrapper function of the IfxAsclin_Asc_blockingWrite function
 * it sends one byte via UART to the receiver
 * @params: uint8, the byte to be transfered
 * @return: void
 */
void uart_blockingWrite(uint8 byte);

/***
 * @brief: a wrapper function of the IfxAsclin_Asc_write function
 * it sends a char array to the receiver
 * @params: uint8 pointer: the char array to be transfered
 * @params: Ifx_SizeT: the size of the char array
 * @return: void
 */
void uart_sendMessage(uint8 *data, Ifx_SizeT size);

/***
 * @brief: a function that sends the 32 bit long serial id of the sensor
 * as a hex value via UART to the receiver
 * @params: uint32, the serial id to be transfered
 * @return: void
 */
void send_serial_id(const uint32 serial_id);

/***
 * @brief: a function that sends the two values read from the sensor
 * via UART to the receiver
 * @params: uint8, the heart rate value to be transfered
 * @params: uint8, the blood oxygen saturation value to be transfered
 * @return: void
 */
void send_values(const uint8 hr, const sint32 spo2);

/***
 * @brief: a function that sends a timestamp
 * via UART to the receiver
 * @params: uint8, the amount of seconds passed to be transfered
 * @params: uint8, the amount of minutes passed to be transfered
 * @params: uint8, the amount of hours passed to be transfered
 * @return: void
 */

void send_timestamp (const uint8 secs, const uint8 mins, const uint8 hours);

#endif /* UART_H_ */
