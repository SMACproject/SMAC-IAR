/******************************************************************************
    Filename: adc.h
    Structure Of the Wirelles Device
    Beccani Marco
    1.0 1/27/2013
******************************************************************************/
#include "adc.h"
/******************************************************************************
* 
* @brief
*      This function makes the adc sample the given channel at the given
*      resolution with the given reference.
*
* @param uint8 reference
*          The reference to compare the channel to be sampled.
*        uint8 resolution
*          The resolution to use during the sample (8, 10, 12 or 14 bit)
*        uint8 input
*          The channel to be sampled.
******************************************************************************/
 void adc_convert(uint8 reference, uint8 resolution, uint8 channel, unsigned char ADC_VALUE[2])
{   
    ADC_SINGLE_CONVERSION(reference | resolution | channel);
    while(!ADCIF);
    ADC_VALUE[1]  = ADCH; 
    ADC_VALUE[0]  = ADCL; 
    ADCIF = 0; 
    
}



