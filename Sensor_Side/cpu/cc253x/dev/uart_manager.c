
/******************************************************************************
    Filename: uart_manager.c 
    uart 
    Beccani Marco
    1.0 2/3/2013
******************************************************************************/

#include "hal_types.h"
#include "hal_defs.h"
#include "hal_mcu.h"  
#include "clock.h"
#include "uart_manager.h"

/***********************************************************************************
* GLOBAL FUNCTIONS
***********************************************************************************/

unsigned char __xdata uartTxBuf[256];
unsigned char __xdata uartRxBuf[256];
unsigned char __xdata uartTxIndex;
unsigned char __xdata uartRxIndex;
extern unsigned char uart_status; 

void uart0_Init(void)
    {
      URX0IE = 1; // UART0 INTERRUPT ENABLE
      URX0IF = 0; // UART0 INT FLAG 
      U0CSR |= 0xC0; // UART Mode Receiver Enabled
      PERCFG &= ~ 0x01; // PERCFG.U0CFG = 0 
      P0SEL |= 0x3C; // P0_5, P1_4, P1_3, and P1_2 are peripherals
      U0UCR = 0x02;  // HIGH Stop Bit
      U0BAUD |= 0x00; //0xD8; // Present 1Mbitbps
      U0GCR = (U0GCR &~ 0x1F) | 0x0F; //   115200 bps
     } 


    _Pragma("vector=0x13") __near_func __interrupt void UART0_RX_ISR(void){ 
     URX0IF = 0; 
      uartRxBuf[0]=U0DBUF;
      IEN0 &= ~0x04;
      if ( uartRxBuf[0] == PC_CONNECT ) {           // P
      uart_status = PC_CONNECTED;
      }
      if ( uartRxBuf[0] == WD_START ) {            // Q
      uart_status = WD_START_REQ;
      }
      if ( uartRxBuf[0] == DEV_SLEEP ) {           // S           
      uart_status = DEV_SLEEP;
      }
      if ( uartRxBuf[0] == ENABLE_ACQUISITION ) {  // E
      uart_status = ENABLE_ACQUISITION;
      }
      if ( uartRxBuf[0] ==START_SCANNING  )  {  // H
      uart_status = START_SCANNING;
      }
      if ( uartRxBuf[0] ==HOLD_ON  )  {  // H
      uart_status = HOLD_ON;
      }
      if ( uartRxBuf[0] ==PAUSE  )  {  // J
      uart_status = PAUSE;
      }
      
      memset(uartRxBuf,0,sizeof(uartRxBuf));
      URX0IE = 1;
      } 

    void uart0_Send(unsigned char* uartTxBuf, unsigned int uartTxBufLength){ 
    unsigned char uartTxIndex; 
      UTX0IF = 0; 
    for (uartTxIndex = 0; uartTxIndex < uartTxBufLength; uartTxIndex++){ 
        U0DBUF = uartTxBuf[uartTxIndex]; 
    while( !UTX0IF ); 
        UTX0IF = 0; 
      } 
    } 