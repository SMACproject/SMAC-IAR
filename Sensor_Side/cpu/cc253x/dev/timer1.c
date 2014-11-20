/******************************************************************************
    Filename: timer1.c

******************************************************************************/

#include <timer1.h>

bool TMR1_Set(uint8 prescaler, uint8 mode, BOOL int_set)
{   if (int_set) {
                  T1_INT_ENABLE(); 
                  T1_OVF_INTMSK();
                 }
    TMR1_CONTROL(prescaler | mode);
    if (mode == T1_MODULO)
      { TMR1_CCTL0( IM | MODE );
      }
return TRUE;
}

bool T1CC0(uint8 T1_cc0h, uint8 T1_cc0l)
{ 
  T1CC0H |= T1_cc0h;
  T1CC0L |= T1_cc0l; 
  return TRUE;
}

void tic(uint8 prescaler) {  
    TMR1_CONTROL(prescaler | T1_FREE_RUNN);
    }

void toc(uint8 prescaler) {
  
    TMR1_CONTROL(prescaler | T1_OP_SUSP); 
    }

void reset_TMR1(unsigned char  * time_t1, int time_t1_sz) {
    time_t1[sizeof(time_t1)-2] = T1CNTL;
    time_t1[sizeof(time_t1)-1] = T1CNTH;
    T1CNTL =0; // Clear Timer Values
    }

  
 #pragma vector=T1_VECTOR 
 __interrupt void TIMER1_IRQ(){ 
    

  }