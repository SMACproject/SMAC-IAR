

/******************************************************************************
    Filename: ads8320.c 
    Structure Of the ADS8320
    Beccani Marco
    1.0 2/09/2013
******************************************************************************/



#include "hal_types.h"
#include "hal_defs.h"
#include "hal_mcu.h"  
#include "ads8320.h"
#include "clock.h"
#include "ioCC2530.h"

/***********************************************************************************
* GLOBAL FUNCTIONS
***********************************************************************************/



void Read_Adc(unsigned char* Adc_val)
{  
  static unsigned char r[4]={0};
  for (int Meas_idx = 0; Meas_idx < 3 ; Meas_idx++) {
  asm("NOP");	// Minor delay of 1 system clock (32MHz)
  U1DBUF =DUMMY; 
  while ( U1CSR & 0x01 ); // Wait for U1ACTIVE to be de-asserted (end of byte transfer)
  r[Meas_idx] = U1DBUF; 
  }
Adc_val[0] = (r[0] << 6) |  (r[1] >> 2);
Adc_val[1] = (r[1] << 6) |  (r[2] >> 2);
} 