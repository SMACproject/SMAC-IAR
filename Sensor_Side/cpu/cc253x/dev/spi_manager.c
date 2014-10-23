/******************************************************************************
    Filename: spi_manager.c 
    Structure Of the SPI
    Beccani Marco
    1.0 2/09/2013
******************************************************************************/


#include "hal_types.h"
#include "hal_defs.h"
#include "hal_mcu.h"  
#include "spi_manager.h"
#include "clock.h"
#include "cc253x.h"


void SPI1_Switch_SSN(uint8 SSN_, bool value){
if (SSN_ == 4) 
{ P1_4 = value; 
P1_3 = HIGH; P1_2 = HIGH; P1_1 = HIGH; P1_0 = HIGH; halMcuWaitUs(2);}
else if (SSN_ == 3) 
{ P1_3 = value;
P1_4 = HIGH; P1_2 = HIGH; P1_1 = HIGH; P1_0 = HIGH; halMcuWaitUs(2);}
else if (SSN_ == 2) 
{ P1_2 = value;
P0_4 = HIGH; P0_3 = HIGH; P0_1 = HIGH; P1_0 = HIGH; halMcuWaitUs(2);}
else if (SSN_ == 1) 
{  P1_1 = value;
P0_4 = HIGH; P1_3 = HIGH; P1_2 = HIGH; P1_0 = HIGH; halMcuWaitUs(2);}
else if (SSN_ == 0) 
{  P1_0 = value;
P0_4 = HIGH; P1_3 = HIGH; P1_2 = HIGH; P1_1 = HIGH; halMcuWaitUs(2);}
}



void SPI1_Send(unsigned char* SPI1TxBuf, unsigned int SPI1TxBufLength) {
unsigned int SPI1TxIndex;
for (SPI1TxIndex = 0; SPI1TxIndex < SPI1TxBufLength; SPI1TxIndex++) {
  U1DBUF = SPI1TxBuf[SPI1TxIndex];
#if defined SDCC || defined (__SDCC)
  while(ACTIVE);
#else
  while(U1ACTIVE);
#endif
}
}

void SPI1_Receive(unsigned char* SPI1RxBuf, unsigned char SPI1RxBufLength) { 
unsigned int SPI1RxIndex; 
for (SPI1RxIndex = 0; SPI1RxIndex < SPI1RxBufLength; SPI1RxIndex++)
  {
  asm("NOP");	// Minor delay of 1 system clock (32MHz)
  U1DBUF =DUMMY; 
  while ( U1CSR & 0x01 ); // Wait for U1ACTIVE to be de-asserted (end of byte transfer)
  SPI1RxBuf[SPI1RxIndex] = U1DBUF; 
  } 
} 

void SPI1_Init(void)
{
// Master Mode
PERCFG = 0x02; // PERCFG.U1CFG = 1
P1SEL |= 0xE0; // P1_7, P1_6, and P1_5 are peripherals
P1SEL &= ~0x18; // P1_4 is GPIO (SSN)
P1DIR |= 0x1F; // SSN is set as output P1_4 to P1_0
// Set baud rate to max (system clock frequency / 8)
// Assuming a 32 MHz crystal (CC1110Fx/CC2510Fx),
// max baud rate = 32 MHz / 8 = 4 MHz.
U1BAUD = 0x7F; // BAUD_M = 216
U1GCR |= 0x10; // BAUD_E = 11
// SPI Master Mode
U1CSR &= ~0xFF;
// Configure phase, polarity, and bit order
U1GCR &= ~0xC0; // CPOL = CPHA = 0
U1GCR |= 0x20; // ORDER = 1
}