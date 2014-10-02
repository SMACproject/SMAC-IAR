/******************************************************************************
    Filename: timer1.c



******************************************************************************/

#include <timer1.h>

void tmr1_set(uint8 prescaler, uint8 mode)
{
    T1_INT_ENABLE();
    T1_OVF_INTMSK();
    TMR1_CONTROL(prescaler | mode);
    if (mode == T1_MODULO)
      { 
      T1CCTL0 = 0x44;
      }
}

void T1_CC0_VAL(uint8 T1_cc0h, uint8 T1_cc0l)
{ 
  T1CC0H |= T1_cc0h;
  T1CC0L |= T1_cc0l; 
}