/*
 * sensor_timer.h
 *
 *  Created on: 20.01.2024
 *      Author: Andreas Reichenauer
 */

#ifndef SENSOR_TIMER_H_
#define SENSOR_TIMER_H_

/*********************************************************************************************************************/
/*---------------------------------------------Type Definitions----------------------------------------------*/
/*********************************************************************************************************************/
typedef void (*interrupt_fptr_t)(void);       // function type used for interrupt handling

/*********************************************************************************************************************/
/*---------------------------------------------Function Definitions----------------------------------------------*/
/*********************************************************************************************************************/
/***
 * @brief: interrupt handler called whenever a read timer interrupt occurs
 * @params: None
 * @return: void
 */
void interruptReadTimer(void);

/***
 * @brief: interrupt handler called whenever an error timer interrupt occurs
 * @params: None
 * @return: void
 */
void interruptErrorTimer(void);

/***
 * @brief: initialises the TOM module used for the read timer and also sets the function that is used inside the interrupt
 * handler for further actions
 * @params: interrupt_fptr_t, the type of function used for detailed interrupt handling
 * @returns: void
 */
void init_read_timer(interrupt_fptr_t interrupt_function_);

/***
 * @brief: initialises the TOM module used for the error timer and also sets the function that is used inside the interrupt
 * handler for further actions
 * @params: interrupt_fptr_t, the type of function used for detailed interrupt handling
 * @returns: void
 */
void init_error_timer(interrupt_fptr_t interrupt_function_);

/***
 * @brief: starts the execution of the read timer
 * @params: none
 * @returns: void
 */
void start_read_timer(void);

/***
 * @brief: starts the execution of the error timer
 * @params: none
 * @returns: void
 */
void start_error_timer(void);

/***
 * @brief: stops the execution of the read timer
 * @params: none
 * @returns: void
 */
void stop_read_timer(void);

/***
 * @brief: stops the execution of the error timer
 * @params: none
 * @returns: void
 */
void stop_error_timer(void);

#endif /* SENSOR_TIMER_H_ */
