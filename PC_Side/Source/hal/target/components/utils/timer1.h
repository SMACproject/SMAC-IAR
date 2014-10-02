/******************************************************************************
    Filename: timer1.h

    Macros for usage of the timer1.

******************************************************************************/

#ifndef _TIMER1_H
#define _TIMER1_H

/*******************************************************************************
 * INCLUDES
 */

#include <hal_types.h>
#include <hal_defs.h>

// Include chip specific IO definition file
#if (chip == 2530 || chip == 2531)
#include <ioCC2530.h>
#endif

/*******************************************************************************
 * CONSTANTS AND MACROS
 */

// Macro for setting up a single conversion. If ADCCON1.STSEL = 11, using this
// macro will also start the conversion.

#define TMR1_CONTROL(settings) st( T1CTL = (settings); )

// Where _settings_ are the following:
// : Prescaler divider value 
#define T1TICK_1         0x00     // Tick Frequency / 1 
#define T1TICK_8         0x04     // Tick Frequency / 8
#define T1TICK_32        0x08     // Tick Frequency / 32
#define T1TICK_128       0x0C     // Tick Frequency / 128

// Mode select:
#define T1_OP_SUSP        0x00     //  Operation suspended
#define T1_FREE_RUNN      0x01     //  Free Running
#define T1_MODULO         0x02     //  Modulo
#define T1_UP_DOWN        0x03     //  Up/Down

#define T1_INT_ENABLE() st( IEN1 |= 0x02;  ) // Enable Timer 1 Interrupt
#define T1_OVF_INTMSK() st (TIMIF |= 0x20;) // Timer1 Overflow Mask
#define TIMER1_RUN(value)   (T1CTL = (value) ? T1CTL|0x02 : T1CTL&~0x03)

/*******************************************************************************
 * PUBLIC FUNCTIONS */

void tmr1_set(uint8 prescaler, uint8 mode);
void T1_CC0_VAL(uint8 T1_cc0h, uint8 T1_cc0l);

/******************************************************************************/

#endif
