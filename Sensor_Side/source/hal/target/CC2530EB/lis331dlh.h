/******************************************************************************
    Filename: lis331dlh.h 
    Structure Of the SPI
    Beccani Marco
    1.0 2/09/2013
******************************************************************************/

#include "hal_types.h"
#include "hal_defs.h"
#include "hal_mcu.h"  
#include "clock.h"
#include "ioCC2530.h"

#ifndef LIS331DLH_H
#define LIS331DLH_H

#ifndef DUMMY
#define DUMMY     0x00;
#endif

#define ACK_LIS331DLH   0x32 // Accellerometer Address
#define WHO_I_AM        0x8F // read 8h
#define CTR1            0x20 // write CTR1
#define XYZ             0xE8 // read all accelleration
#define Xh              0xA9 // read ax_h
#define Xl              0xA8 // read ax_l
#define Yh              0xAB // read ay_h
#define Yl              0xAA // read ay_l
#define Zh              0xAD // read az_h
#define Zl              0xAC // read az_l


void Accell_Init(void);
bool Accell_Who_I_AM(void);
void Read_accelleration(unsigned char* acc_val);

#endif