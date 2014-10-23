
/******************************************************************************
    Filename: pwr_mode_manager.c 
    Structure Of the Wirelles Device
    Beccani Marco
    1.0 1/27/2013
******************************************************************************/

#include "hal_types.h"
#include "hal_defs.h"
#include "hal_mcu.h"  
#include "pwr_mode_manager.h"
#include "clock.h"

/*******************************************************************************/

/***********************************************************************************
* GLOBAL FUNCTIONS
***********************************************************************************/

    void SleepTimerInit(int seconds){
    unsigned long sleeptime = 0;
    sleeptime |= ST0;
    sleeptime |= (unsigned long)ST1 << 8;
    sleeptime |= (unsigned long)ST2 << 16;
    sleeptime += ((unsigned long) seconds * (unsigned long) 32000); // 32 kHz
    /* set sleep timer */
    while((STLOAD & 0x01) == 0); // wait before ST0. STLOAD.LDRDY is 0 during the load
    ST2 = (unsigned char)(sleeptime >> 16);
    ST1 = (unsigned char)(sleeptime >> 8);
    ST0 = (unsigned char) sleeptime;
    STIE=1;
    IEN0 |= 0x20;
    EA = 1; // Enable global interrupt
   
  }

void halMcuSetLowPowerMode(uint8 mode){
    switch (mode){
    case 0x00:
        SLEEPCMD &= IDLE;
        SLEEPCMD |= IDLE;
       
        break;
    case 0x02:
        SLEEPCMD &= IDLE;
        SLEEPCMD |= PM2;
        PCON |= 0x01; 
        break;
    case 0x03:
        SLEEPCMD &= IDLE;
        SLEEPCMD |= PM3;
        PCON |= 0x01; 
        break;
    }  
  }


#pragma vector=ST_VECTOR
  __interrupt void sleeptimer_ISR(){
    unsigned long sleeptime = 0;
    STIF = 0; //clear interrupt flag
    sleeptime |= ST0;
    sleeptime |= (unsigned long)ST1 << 8;
    sleeptime |= (unsigned long)ST2 << 16;
    sleeptime += ((unsigned long) 3 * (unsigned long)32000);
    /* set sleep timer */
    while((STLOAD & 0x01) == 0); // wait before ST0. STLOAD.LDRDY is 0 during the load
    ST2 = (unsigned char)(sleeptime >> 16);
    ST1 = (unsigned char)(sleeptime >> 8);
    ST0 = (unsigned char) sleeptime;
  }

#if defined(SDCC)|| defined(__SDCC)
void PORT1_IRQ() __interrupt(P1INT_VECTOR)
#else
#pragma vector_1=P1INT_VECTOR              //need to check:function has overlaid vector
__near_func __interrupt void PORT1_IRQ()
#endif
{
  //P1IFG = ~(1 << 2);
  //IRCON2 = ~(1 << 3); 
}
  
#if defined(SDCC)|| defined(__SDCC)
void PORT0_IRQ() __interrupt(P0INT_VECTOR)
#else
#pragma vector_0=P0INT_VECTOR             //need to check:function has overlaid vector
__near_func __interrupt void PORT0_IRQ()
#endif
{
  P0IFG = ~(1 << 0);
  IRCON = ~(1 << 5); 
}
  