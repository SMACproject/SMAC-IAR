/******************************************************************************
    Filename: spi_manager.h 
    Structure Of the SPI
    Beccani Marco
    1.0 2/09/2013
******************************************************************************/

#ifndef SPI_MANAGER_H
#define SPI_MANAGER_H

#include "hal_types.h"
#include "hal_defs.h"
#include "hal_mcu.h"  
#include "clock.h"
#include "cc253x.h"


#ifndef DUMMY
#define DUMMY     0x00;
#endif



/***********************************************************************************
* GLOBAL FUNCTIONS
*/

void SPI1_Switch_SSN(uint8 SSN_, bool value);
void SPI1_Send(unsigned char* SPI1TxBuf, unsigned int SPI1TxBufLength);
void SPI1_Receive(unsigned char* SPI1RxBuf, unsigned char SPI1RxBufLength);
void SPI1_Init(void);



#endif