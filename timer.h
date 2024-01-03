/*
 * timer.h
 *
 *  Created on: 10.12.2023
 *      Author: Andreas Reichenauer
 */

#ifndef TIMER_H_
#define TIMER_H_

/*********************************************************************************************************************/
/*---------------------------------------------Type Definitions----------------------------------------------*/
/*********************************************************************************************************************/
typedef void (*interrupt_fptr_t)(void);       // function type used for interrupt handling

/*********************************************************************************************************************/
/*---------------------------------------------Function Definitions----------------------------------------------*/
/*********************************************************************************************************************/
/***
 * @brief: interrupt handler called whenever a timer interrupt occurs
 * @params: None
 * @return: void
 */
void interruptGtmTom(void);

/***
 * @brief: initialises the TOM module used as a Timer and also sets the function that is used inside the interrupt
 * handler for further actions
 * @params: interrupt_fptr_t, the type of function used for detailed interrupt handling
 * @returns: void
 */
void init_timer(interrupt_fptr_t interrupt_function_);

#endif /* TIMER_H_ */
