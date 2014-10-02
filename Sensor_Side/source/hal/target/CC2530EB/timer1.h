/******************************************************************************
    Filename: timer1.h
    Macros for usage of the timer1.
    @uthor Beccani Marco

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

#define TMR1_CCTL0(settings) st( T1CCTL0 = (settings); )
#define TMR1_CCTL1(settings) st( T1CCTL1 = (settings); )
#define TMR1_CCTL2(settings) st( T1CCTL2 = (settings); )
#define TMR1_CCTL3(settings) st( T1CCTL3 = (settings); )
#define TMR1_CCTL4(settings) st( T1CCTL4 = (settings); )

// Where _settings_ are the following:

#define RFIRQ            0x80     // RF interrupt for capture input
#define IM               0x40     // Ch0 Int Mask
#define SET_CMP_OUT      0x00     // Set Output on Compare
#define CLR_CMP_OUT      0x08     // Clear Output on Compare
#define TGL_CMP_OUT      0x10     // Toggle Output on Compare
#define SET_CMP_OUT_C0   0x18     // Set Output on Compare Clear on 0
#define CLR_CMP_OUT_S0   0x20     // Clear Output on Compare Set on 0
#define SET_OUT_PIN      0x38     // Initialize Output pin
#define MODE             0x04     // Compare Mode 
#define CAP_11           0x03     // Capture all edges
#define CAP_10           0x02     // Capture Falling
#define CAP_01           0x01     // Capture Rising
#define CAP_00           0x00     // No Capture

#define T1_INT_ENABLE() st( IEN1 |= 0x02;  ) // Enable Timer 1 Interrupt
#define T1_OVF_INTMSK() st (TIMIF |= 0x20;) // Timer1 Overflow Mask
#define TIMER1_RUN(value)   (T1CTL = (value) ? T1CTL|0x02 : T1CTL&~0x03)

/*******************************************************************************
 * PUBLIC FUNCTIONS */

bool TMR1_Set(uint8 prescaler, uint8 mode, BOOL int_set);
bool T1CC0(uint8 T1_cc0h, uint8 T1_cc0l);

void toc(uint8 prescaler);
void tic(uint8 prescaler);

void reset_TMR1(unsigned char  * time_t1, int time_t1_sz);

/******************************************************************************/

#endif
