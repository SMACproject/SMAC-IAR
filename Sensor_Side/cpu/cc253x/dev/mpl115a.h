#ifndef MPL115A_H
#define MPL115A_H

#ifndef DUMMY
#define DUMMY     0x00;
#endif

#include "hal_types.h"
#include "hal_defs.h"
#include "hal_mcu.h"  
#include "clock.h"
#include "cc253x.h"
#include "spi_manager.h"
#include "bin_cnst.h"
#include "string.h"

// Pressure Sensor Commands
#define CMD_a0h   0x88;
#define CMD_a0l   0x8A;
#define CMD_b1h   0x8C;
#define CMD_b1l   0x8E;
#define CMD_b2h   0x90;
#define CMD_b2l   0x92;
#define CMD_c12h  0x94;
#define CMD_c12l  0x8A;
#define CMD_MEASURE  0x24;
#define CMD_Ph  0x80;
#define CMD_Pl  0x82;
#define CMD_Th  0x84;
#define CMD_Tl  0x86;

// Float Numbers Manipulation
#define LOB(x) (*((unsigned char *)&(x)))
#define M1B(x) (*((unsigned char *)&(x)+1))
#define M2B(x) (*((unsigned char *)&(x)+2))
#define HIB(x) (*((unsigned char *)&(x)+3))


float a0,b1,b2,c12;
unsigned char a0h, a0l, b1h, b1l, b2h, b2l ,c12h, c12l;
unsigned char Ph, Pl, Th, Tl;

/***********************************************************************************
* GLOBAL FUNCTIONS
***********************************************************************************/


float a0_comp(unsigned char a_h, unsigned char a_l);
float b1_comp(unsigned char b_h, unsigned char b_l);
float b2_comp(unsigned char b2_h, unsigned char b2_l);
float c12_comp(unsigned char c12_h, unsigned char c12_l);
void Read_Coefficients(unsigned int Coeff_Number, unsigned char* coefficients_);
void Read_Pressure(unsigned char* pressure);
void Meas_Init(void);


#endif