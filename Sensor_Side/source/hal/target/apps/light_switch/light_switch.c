#include <ioCC2530.h>
#include <hal_assert.h>
#include <hal_board.h>
#include <hal_int.h>
#include <hal_mcu.h>
#include <util_lcd.h>
#include <adc.h>
#include <hal_lcd.h>
#include <hal_led.h>
#include <hal_types.h>
#include <hal_int.h>
#include <timer1.h>
#include <stdio.h>
#include <string.h>

#define LSB_12 0.001611328125
char s[16];
float vol = 0;

void T1_Init(void);

void main(void) {
  
  halBoardInit(); 
  halLcdClear();
  T1_Init();
  while(1) { 
   halLedToggle(3);
    }
}

void T1_Init(void)
{     
  tmr1_set(T1TICK_128, T1_MODULO);
  T1CNTH = 0X00;
  T1CNTL = 0X00;
  T1_CC0_VAL(0X7F ,0x7F);
  T1_INT_ENABLE();
}

#pragma vector=T1_VECTOR 
__interrupt void TIMER1_IRQ()
{   
    vol=adcSampleSingle(ADC_REF_AVDD, ADC_12_BIT, ADC_AIN7);
    sprintf(s, (char*)"%f V", vol* LSB_12);
    halLcdWriteLine(1,s);
    T1CNTH = 0X00;
    T1CNTL = 0X00;
}


     
     

