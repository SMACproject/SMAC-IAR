
/******************************************************************************
    Filename: pwr_mode_manager.h 
    Structure Of the Wirelles Device
    Beccani Marco
    1.0 1/27/2013
******************************************************************************/

#ifndef UART_MANAGER_H
#define UART_MANAGER_H

#include "hal_types.h"
#include "hal_defs.h"
#include "hal_mcu.h"  
#include "ioCC2530.h"
#include <stdio.h>
#include <stdlib.h>
#include <ioCC2530.h>
#include <string.h>

// UART Commands 
#define PC_CONNECT           'P'
#define PC_CONNECTED         'p'
#define WD_START             'Q'
#define WD_START_REQ         'q'
#define DEV_SLEEP            'S'
#define SLEEP_ACK            's'
#define ENABLE_ACQUISITION                'E'    
#define ENABLE_ACQUISITION_ACK            'e'
#define START_SCANNING         'H'
#define HOLD_ON 'h'
#define PAUSE   'J'

/***********************************************************************************
* GLOBAL FUNCTIONS
***********************************************************************************/
void uart0_Init(void);
void uart0_Send(unsigned char* uartTxBuf, unsigned int uartTxBufLength);

#endif