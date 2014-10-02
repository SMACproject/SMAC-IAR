
/******************************************************************************
    Filename: lis331dlh.c 
    Structure Of the ADS8320
    Beccani Marco
    1.0 2/09/2013
******************************************************************************/

#include "hal_types.h"
#include "hal_defs.h"
#include "hal_mcu.h"  
#include "clock.h"
#include "ioCC2530.h"
#include "lis331dlh.h"


/***********************************************************************************
* GLOBAL FUNCTIONS
***********************************************************************************/


void Accell_Init(void)
{
  P1_0 = LOW;
  U1DBUF = CTR1;
  while(U1ACTIVE);
  U1DBUF = 0x2F;
  while(U1ACTIVE);
  P1_0 = HIGH;
}

bool Accell_Who_I_AM(void)
{ 
  bool device_id = FALSE;
  P1_0 = LOW;
  U1DBUF = WHO_I_AM;
  while(U1ACTIVE);
  asm("NOP");	// Minor delay of 1 system clock (32MHz)
  U1DBUF =DUMMY;
  while ( U1CSR & 0x01 ); // Wait for U1ACTIVE to be de-asserted (end of byte transfer)
  device_id = U1DBUF; 
  P1_0 = HIGH;
  if (device_id == ACK_LIS331DLH) 
    return TRUE;
  else return FALSE;
}

void Read_accelleration(unsigned char* acc_val)
{
  P1_0 = LOW;
  U1DBUF = XYZ;
  while(U1ACTIVE);
  for (int acc_idx = 0; acc_idx < 6 ; acc_idx++){
  asm("NOP");	// Minor delay of 1 system clock (32MHz)
  U1DBUF =DUMMY;
  while ( U1CSR & 0x01 ); // Wait for U1ACTIVE to be de-asserted (end of byte transfer)
  acc_val[acc_idx] = U1DBUF; 
  }
  P1_0 = HIGH;
}
