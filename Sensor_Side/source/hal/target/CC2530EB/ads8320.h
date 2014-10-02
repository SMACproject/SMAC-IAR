

/******************************************************************************
    Filename: ads8320.h 
    Structure Of the ADS8320
    Beccani Marco
    1.0 2/09/2013
******************************************************************************/

#ifndef ADS8320_H
#define ADS8320_H

#ifndef DUMMY
#define DUMMY     0x00;
#endif


#include "hal_types.h"
#include "hal_defs.h"
#include "hal_mcu.h"  
#include "ioCC2530.h"

/*******************************************************************************/

// ADC Api
void Read_Adc(unsigned char* Adc_val);



#endif