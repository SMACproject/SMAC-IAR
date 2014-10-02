/**************************************************************************************************
  Filename:       _hal_uart.c
  Revised:        $Date: 2009-06-12 09:16:43 -0700 (Fri, 12 Jun 2009) $
  Revision:       $Revision: 20142 $

  Description: This file contains the interface to the H/W UART driver.


  Copyright 2006-2009 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights
  granted under the terms of a software license agreement between the user
  who downloaded the software, his/her employer (which must be your employer)
  and Texas Instruments Incorporated (the "License").  You may not use this
  Software unless you agree to abide by the terms of the License. The License
  limits your use, and you acknowledge, that the Software may not be modified,
  copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio
  frequency transceiver, which is integrated into your product.  Other than for
  the foregoing purpose, you may not use, reproduce, copy, prepare derivative
  works of, modify, distribute, perform, display or sell this Software and/or
  its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
  PROVIDED “AS IS” WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
  INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
  NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
  TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
  LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
  INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
  OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
  OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
  (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/

/*********************************************************************
 * INCLUDES
 */

#include "hal_defs.h"
#include "hal_types.h"
#include "hal_uart.h"

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * TYPEDEFS
 */

/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * GLOBAL VARIABLES
 */

/*********************************************************************
 * GLOBAL FUNCTIONS
 */

/*********************************************************************
 * LOCAL VARIABLES
 */

/*********************************************************************
 * LOCAL FUNCTIONS
 */

#if HAL_UART_DMA
#include "_hal_uart_dma.c"
#endif
#if HAL_UART_ISR
#include "_hal_uart_isr.c"
#endif
#if HAL_UART_USB
#include "_hal_uart_usb.c"
#endif

/******************************************************************************
 * @fn      HalUARTInit
 *
 * @brief   Initialize the UART
 *
 * @param   none
 *
 * @return  none
 *****************************************************************************/
void HalUARTInit(void)
{
#if HAL_UART_DMA
  HalUARTInitDMA();
#endif
#if HAL_UART_ISR
  HalUARTInitISR();
#endif
#if HAL_UART_USB
  HalUARTInitUSB();
#endif
}

/******************************************************************************
 * @fn      HalUARTOpen
 *
 * @brief   Open a port according tp the configuration specified by parameter.
 *
 * @param   port   - UART port
 *          config - contains configuration information
 *
 * @return  Status of the function call
 *****************************************************************************/
uint8 HalUARTOpen(uint8 port, halUARTCfg_t *config)
{
  (void)port;
  (void)config;

#if (HAL_UART_DMA == 1)
  if (port == HAL_UART_PORT_0)  HalUARTOpenDMA(config);
#endif
#if (HAL_UART_DMA == 2)
  if (port == HAL_UART_PORT_1)  HalUARTOpenDMA(config);
#endif
#if (HAL_UART_ISR == 1)
  if (port == HAL_UART_PORT_0)  HalUARTOpenISR(config);
#endif
#if (HAL_UART_ISR == 2)
  if (port == HAL_UART_PORT_1)  HalUARTOpenISR(config);
#endif
#if (HAL_UART_USB)
  HalUARTOpenUSB(config);
#endif
  
  return HAL_UART_SUCCESS;
}

/*****************************************************************************
 * @fn      HalUARTRead
 *
 * @brief   Read a buffer from the UART
 *
 * @param   port - USART module designation
 *          buf  - valid data buffer at least 'len' bytes in size
 *          len  - max length number of bytes to copy to 'buf'
 *
 * @return  length of buffer that was read
 *****************************************************************************/
uint16 HalUARTRead(uint8 port, uint8 *buf, uint16 len)
{
  (void)port;
  (void)buf;
  (void)len;

#if (HAL_UART_DMA == 1)
  if (port == HAL_UART_PORT_0)  return HalUARTReadDMA(buf, len);
#endif
#if (HAL_UART_DMA == 2)
  if (port == HAL_UART_PORT_1)  return HalUARTReadDMA(buf, len);
#endif
#if (HAL_UART_ISR == 1)
  if (port == HAL_UART_PORT_0)  return HalUARTReadISR(buf, len);
#endif
#if (HAL_UART_ISR == 2)
  if (port == HAL_UART_PORT_1)  return HalUARTReadISR(buf, len);
#endif

#if HAL_UART_USB
  return HalUARTRx(buf, len);
#else
  return 0;
#endif
}

/******************************************************************************
 * @fn      HalUARTWrite
 *
 * @brief   Write a buffer to the UART.
 *
 * @param   port - UART port
 *          buf  - pointer to the buffer that will be written, not freed
 *          len  - length of
 *
 * @return  length of the buffer that was sent
 *****************************************************************************/
uint16 HalUARTWrite(uint8 port, uint8 *buf, uint16 len)
{
  (void)port;
  (void)buf;
  (void)len;

#if (HAL_UART_DMA == 1)
  if (port == HAL_UART_PORT_0)  return HalUARTWriteDMA(buf, len);
#endif
#if (HAL_UART_DMA == 2)
  if (port == HAL_UART_PORT_1)  return HalUARTWriteDMA(buf, len);
#endif
#if (HAL_UART_ISR == 1)
  if (port == HAL_UART_PORT_0)  return HalUARTWriteISR(buf, len);
#endif
#if (HAL_UART_ISR == 2)
  if (port == HAL_UART_PORT_1)  return HalUARTWriteISR(buf, len);
#endif

#if HAL_UART_USB
  HalUARTTx(buf, len);
  return len;
#else
  return 0;
#endif
}

/******************************************************************************
 * @fn      HalUARTSuspend
 *
 * @brief   Suspend UART hardware before entering PM mode 1, 2 or 3.
 *
 * @param   None
 *
 * @return  None
 *****************************************************************************/
void HalUARTSuspend( void )
{
#if HAL_UART_DMA
  HalUARTSuspendDMA();
#endif
#if HAL_UART_ISR
  HalUARTSuspendISR();
#endif
}

/******************************************************************************
 * @fn      HalUARTResume
 *
 * @brief   Resume UART hardware after exiting PM mode 1, 2 or 3.
 *
 * @param   None
 *
 * @return  None
 *****************************************************************************/
void HalUARTResume( void )
{
#if HAL_UART_DMA
  HalUARTResumeDMA();
#endif
#if HAL_UART_ISR
  HalUARTResumeISR();
#endif
}

/***************************************************************************************************
 * @fn      HalUARTPoll
 *
 * @brief   Poll the UART.
 *
 * @param   none
 *
 * @return  none
 *****************************************************************************/
void HalUARTPoll(void)
{
#if HAL_UART_DMA
  HalUARTPollDMA();
#endif
#if HAL_UART_ISR
  HalUARTPollISR();
#endif
#if HAL_UART_USB
  HalUARTPollUSB();
#endif
}

/**************************************************************************************************
 * @fn      Hal_UART_RxBufLen()
 *
 * @brief   Calculate Rx Buffer length - the number of bytes in the buffer.
 *
 * @param   port - UART port
 *
 * @return  length of current Rx Buffer
 **************************************************************************************************/
uint16 Hal_UART_RxBufLen( uint8 port )
{
  (void)port;

#if (HAL_UART_DMA == 1)
  if (port == HAL_UART_PORT_0)  return HalUARTRxAvailDMA();
#endif
#if (HAL_UART_DMA == 2)
  if (port == HAL_UART_PORT_1)  return HalUARTRxAvailDMA();
#endif
#if (HAL_UART_ISR == 1)
  if (port == HAL_UART_PORT_0)  return HalUARTRxAvailISR();
#endif
#if (HAL_UART_ISR == 2)
  if (port == HAL_UART_PORT_1)  return HalUARTRxAvailISR();
#endif
#if HAL_UART_USB
  return HalUARTRxAvailUSB();
#else
  return 0;
#endif
}


void uartInitBitrate(unsigned char uartBaudM, unsigned char uartBaudE) { 
  /////////////////////////////////////////////////////////////// 
  // This initial code section ensures that the SoC system clock is driven 
  // by the HS XOSC: 
  // Clear CLKCON.OSC to make the SoC operate on the HS XOSC. 
  // Set CLKCON.TICKSPD/CLKSPD = 000 => system clock speed = HS RCOSC speed. 
  //CLKCON &= 0x80; 
  // Monitor CLKCON.OSC to ensure that the HS XOSC is stable and actually 
  // applied as system clock source before continuing code execution 
 // while(CLKCON & 0x40); 
  // Set SLEEP.OSC_PD to power down the HS RCOSC. 
  //SLEEP |= 0x04; 
  /////////////////////////////////////////////////////////////// 
  // Initialize bitrate (U0BAUD.BAUD_M, U0GCR.BAUD_E) 
  U0BAUD = uartBaudM; 
  U0GCR = (U0GCR&~0x1F) | uartBaudE; 
} 

void uartInitProtocol(UART_PROT_CONFIG* uartProtConfig) { 
  // Initialize UART protocol for desired UART (0 or 1) 
if (uartProtConfig->uartNum == 0) { 
    // USART mode = UART (U0CSR.MODE = 1) 
    U0CSR |= 0x80; 
    // Start bit level = low => Idle level = high (U0UCR.START = 0) 
    // Start bit level = high => Idle level = low (U0UCR.START = 1) 
    U0UCR = (U0UCR&~0x01) | uartProtConfig->START; 
    // Stop bit level = high (U0UCR.STOP = 1) 
    // Stop bit level = low (U0UCR.STOP = 0) 
    U0UCR = (U0UCR&~0x02) | (uartProtConfig->STOP << 1); 
    // Number of stop bits = 1 (U0UCR.SPB = 0) 
    // Number of stop bits = 2 (U0UCR.SPB = 1) 
    U0UCR = (U0UCR&~0x04) | (uartProtConfig->SPB << 2); 
    // Parity = disabled (U0UCR.PARITY = 0) 
    // Parity = enabled (U0UCR.PARITY = 1) 
    U0UCR = (U0UCR&~0x08) | (uartProtConfig->PARITY << 3); 
    // 9-bit data disable = 8 bits transfer (U0UCR.BIT9 = 0) 
    // 9-bit data enable = 9 bits transfer (U0UCR.BIT9 = 1) 
    U0UCR = (U0UCR&~0x10) | (uartProtConfig->BIT9 << 4); 
    // Level of bit 9 = 0 (U0UCR.D9 = 0), used when U0UCR.BIT9 = 1 
    // Level of bit 9 = 1 (U0UCR.D9 = 1), used when U0UCR.BIT9 = 1 
    // Parity = Even (U0UCR.D9 = 0), used when U0UCR.PARITY = 1 
    // Parity = Odd (U0UCR.D9 = 1), used when U0UCR.PARITY = 1 
    U0UCR = (U0UCR&~0x20) | (uartProtConfig->D9 << 5); 
    // Flow control = disabled (U0UCR.FLOW = 0) 
    // Flow control = enabled (U0UCR.FLOW = 1) 
    U0UCR = (U0UCR&~0x40) | (uartProtConfig->FLOW << 6); 
    // Bit order = MSB first (U0GCR.ORDER = 1) 
    // Bit order = LSB first (U0GCR.ORDER = 0) => For PC/Hyperterminal 
    U0GCR = (U0GCR&~0x20) | (uartProtConfig->ORDER << 5); 
  } else { 
    // USART mode = UART (U1CSR.MODE = 1) 
    U1CSR |= 0x80; 
    // Start bit level = low => Idle level = high (U1UCR.START = 0) 
    // Start bit level = high => Idle level = low (U1UCR.START = 1) 
    U1UCR = (U1UCR&~0x01) | uartProtConfig->START; 
    // Stop bit level = high (U1UCR.STOP = 1) 
    // Stop bit level = low (U1UCR.STOP = 0) 
    U1UCR = (U1UCR&~0x02) | (uartProtConfig->STOP << 1); 
    // Number of stop bits = 1 (U1UCR.SPB = 0) 
    // Number of stop bits = 2 (U1UCR.SPB = 1) 
    U1UCR = (U1UCR&~0x04) | (uartProtConfig->SPB << 2); 
    // Parity = disabled (U1UCR.PARITY = 0) 
    // Parity = enabled (U1UCR.PARITY = 1) 
    U1UCR = (U1UCR&~0x08) | (uartProtConfig->PARITY << 3); 
    // 9-bit data enable = 8 bits transfer (U1UCR.BIT9 = 0) 
    // 9-bit data enable = 8 bits transfer (U1UCR.BIT9 = 1) 
    U1UCR = (U1UCR&~0x10) | (uartProtConfig->BIT9 << 4); 
    // Level of bit 9 = 0 (U1UCR.D9 = 0), used when U1UCR.BIT9 = 1 
    // Level of bit 9 = 1 (U1UCR.D9 = 1), used when U1UCR.BIT9 = 1 
    // Parity = Even (U1UCR.D9 = 0), used when U1UCR.PARITY = 1 
    // Parity = Odd (U1UCR.D9 = 1), used when U1UCR.PARITY = 1 
    U1UCR = (U1UCR&~0x20) | (uartProtConfig->D9 << 5); 
    // Flow control = disabled (U1UCR.FLOW = 0) 
    // Flow control = enabled (U1UCR.FLOW = 1) 
    U1UCR = (U1UCR&~0x40) | (uartProtConfig->FLOW << 6); 
    // Bit order = MSB first (U1GCR.ORDER = 1) 
    // Bit order = LSB first (U1GCR.ORDER = 0) => For PC/Hyperterminal 
    U1GCR = (U1GCR&~0x20) | (uartProtConfig->ORDER << 5); 
  } 
}

void uart0Send(unsigned short* uartTxBuf, unsigned short uartTxBufLength) { 
unsigned short uartTxIndex; 
  UTX0IF = 0; 
for (uartTxIndex = 0; uartTxIndex < uartTxBufLength; uartTxIndex++) { 
    U0DBUF = uartTxBuf[uartTxIndex]; 
while( !UTX0IF ); 
    UTX0IF = 0; 
  } 
} 
void uart1Send(unsigned short* uartTxBuf, unsigned short uartTxBufLength) { 
unsigned short uartTxIndex; 
  UTX1IF = 0; 
for (uartTxIndex = 0; uartTxIndex < uartTxBufLength; uartTxIndex++) { 
    U1DBUF = uartTxBuf[uartTxIndex]; 
while( !UTX1IF ); 
    UTX1IF = 0; 
  } 
} 



/******************************************************************************
******************************************************************************/
