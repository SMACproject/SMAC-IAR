
/******************************************************************************
    Filename: pwr_mode_manager.h 
    Structure Of the Wirelles Device
    Beccani Marco
    1.0 1/27/2013
******************************************************************************/

#ifndef PWR_MODE_MANAGER_H
#define PWR_MODE_MANAGER_H

#include "hal_types.h"
#include "hal_defs.h"
#include "hal_mcu.h"  
#include "ioCC2530.h"

#ifndef     IDLE 
#define     IDLE   0x04;
#endif

#ifndef     PM2 
#define     PM2    0x06;
#endif 

#ifndef     PM3 
#define     PM3    0x07;
#endif 

/***********************************************************************************
* GLOBAL FUNCTIONS
***********************************************************************************/

void SleepTimerInit(int seconds);
void halMcuSetLowPowerMode(uint8 mode);
#endif