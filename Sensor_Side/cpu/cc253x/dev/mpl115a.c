
/******************************************************************************
    Filename: mpl115a.c 
    Structure Of the mpl115a
    Beccani Marco
    1.0 2/09/2013
******************************************************************************/

#include "hal_types.h"
#include "hal_defs.h"
#include "hal_mcu.h"  
#include "mpl115a.h"
#include "clock.h"
#include "cc253x.h"
#include "spi_manager.h"
#include "bin_cnst.h"
#include "string.h"


float a0_comp(unsigned char a_h, unsigned char a_l){
       int merge_byte = (a_h << 8) | a_l;
       a0=0;
       int bit = merge_byte & (1 << 15);
       if (bit==0) {
       bit = merge_byte & (1 << 14);  
       if (bit==16384) a0 = a0 + dec11;
       bit = merge_byte & (1 << 13);  
       if (bit==16384>>1) a0 = a0 + dec10;
       bit = merge_byte & (1 << 12);  
       if (bit==16384>>2) a0 = a0 + dec9;
       bit = merge_byte & (1 << 11);  
       if (bit==16384>>3) a0 = a0 + dec8;
       bit = merge_byte & (1 << 10);  
       if (bit==16384>>4) a0 = a0 + dec7; 
       bit = merge_byte & (1 << 9);  
       if (bit==16384>>5) a0 = a0 + dec6;  
       bit = merge_byte & (1 << 8);  
       if (bit==16384>>6) a0 = a0 + dec5;
       bit = merge_byte & (1 << 7);  
       if (bit==16384>>7) a0 = a0 + dec4;
       bit = merge_byte & (1 << 6);  
       if (bit==16384>>8) a0 = a0 + dec3; 
       bit = merge_byte & (1 << 5);  
       if (bit==16384>>9) a0 = a0 + dec2;  
       bit = merge_byte & (1 << 4);  
       if (bit==16384>>10) a0 = a0 + dec1;  
       bit = merge_byte & (1 << 3);  
       if (bit==16384>>11) a0 = a0 + dec1;  
       bit = merge_byte & (1 << 2);  
       if (bit==16384>>12) a0 = a0 + frac0;  
       bit = merge_byte & (1 << 1);  
       if (bit==16384>>13) a0 = a0 + frac1;  
       bit = merge_byte;  
       if (bit==16384>>14) a0 = a0 + frac2;  
       }
  merge_byte = 0;
  return a0;
  }



float b1_comp(unsigned char b_h, unsigned char b_l){
       int merge_byte = (b_h << 8) | b_l;
       b1=0;
       int bit = merge_byte & (1 << 15);
       if (bit==0) {
       b1=0; 
       // Something wrong this coefficient should be negative all the time
       }
       else {
       bit = merge_byte & (1 << 14);  
       if (bit==0) b1 = b1 - dec1;
       bit = merge_byte & (1 << 13);  
       if (bit==0) b1 = b1 - dec0;
        bit = merge_byte & (1 << 12);  
       if (bit==0) b1 = b1 - frac0;
        bit = merge_byte & (1 << 11);  
       if (bit==0) b1 = b1 - frac1;    
        bit = merge_byte & (1 << 10);  
       if (bit==0) b1 = b1 - frac2;    
        bit = merge_byte & (1 << 9);  
       if (bit==0) b1 = b1 - frac3;   
        bit = merge_byte & (1 << 8);  
       if (bit==0) b1 = b1 - frac4;   
        bit = merge_byte & (1 << 7);  
       if (bit==0) b1 = b1 - frac5;     
        bit = merge_byte & (1 << 6);  
       if (bit==0) b1 = b1 - frac6;  
        bit = merge_byte & (1 << 5);  
       if (bit==0) b1 = b1 - frac7;     
        bit = merge_byte & (1 << 4);  
       if (bit==0) b1 = b1 - frac8;   
        bit = merge_byte & (1 << 3);  
       if (bit==0) b1 = b1 - frac9;
        bit = merge_byte & (1 << 2);  
       if (bit==0) b1 = b1 - frac10;  
       bit = merge_byte & (1 << 1);  
       if (bit==0) b1 = b1 - frac11; 
        bit = merge_byte;  
       if (bit==0) b1 = b1 - frac12; 
       }
  merge_byte = 0;
  return b1;
  }


  float b2_comp(unsigned char b2_h, unsigned char b2_l){
       int merge_byte = (b2_h << 8) | b2_l;
       b2=0;
       int bit = merge_byte & (1 << 15);
       if (bit==0) { 
         b2=0; 
       // Something wrong this coefficient should be negative all the time
       }
       else {
       bit = merge_byte & (1 << 14);  
       if (bit==0) b2 = b2 - dec0;
       bit = merge_byte & (1 << 13);  
       if (bit==0) b2 = b2 - frac0;
        bit = merge_byte & (1 << 12);  
       if (bit==0) b2 = b2 - frac1;
        bit = merge_byte & (1 << 11);  
       if (bit==0) b2 = b2 - frac2;    
        bit = merge_byte & (1 << 10);  
       if (bit==0) b2 = b2 - frac3;    
        bit = merge_byte & (1 << 9);  
       if (bit==0) b2 = b2 - frac4;   
        bit = merge_byte & (1 << 8);  
       if (bit==0) b2 = b2 - frac6;   
        bit = merge_byte & (1 << 7);  
       if (bit==0) b2 = b2 - frac6;     
        bit = merge_byte & (1 << 6);  
       if (bit==0) b2 = b2 - frac7;  
        bit = merge_byte & (1 << 5);  
       if (bit==0) b2 = b2 - frac8;     
        bit = merge_byte & (1 << 4);  
       if (bit==0) b2 = b2 - frac9;   
        bit = merge_byte & (1 << 3);  
       if (bit==0) b2 = b2 - frac10;    
        bit = merge_byte & (1 << 2);  
       if (bit==0) b2 = b2 - frac11;    
        bit = merge_byte & (1 << 1);  
       if (bit==0) b2 = b2 - frac12;  
        bit = merge_byte;  
       if (bit==0) b2 = b2 - frac13; 
       }
  merge_byte = 0;
  return b2;
  }


  float c12_comp(unsigned char c12_h, unsigned char c12_l){
    int merge_byte = (c12_h << 8) | c12_l;
    merge_byte = merge_byte >> 2;
    c12=0;
    int bit = merge_byte & (1 << 13);
    if (bit==0) {
    bit = merge_byte & (1 << 12);  
       if (bit==4096) c12 = c12 + frac9;
    bit = merge_byte & (1 << 11);  
       if (bit==4096>>1) c12 = c12 + frac10;  
    bit = merge_byte & (1 << 10);  
       if (bit==4096>>2) c12 = c12 + frac11;    
    bit = merge_byte & (1 << 9);  
       if (bit==4096>>3) c12 = c12 + frac12;  
    bit = merge_byte & (1 << 8);  
       if (bit==4096>>4) c12 = c12 + frac13;  
    bit = merge_byte & (1 << 7);  
       if (bit==4096>>5) c12 = c12 + frac14;  
    bit = merge_byte & (1 << 6);  
       if (bit==4096>>6) c12 = c12 + frac15;  
    bit = merge_byte & (1 << 5);  
       if (bit==4096>>7) c12 = c12 + frac16;
    bit = merge_byte & (1 << 4);  
       if (bit==4096>>8) c12 = c12 + frac17;
    bit = merge_byte & (1 << 3);  
       if (bit==4096>>9) c12 = c12 + frac18;
    bit = merge_byte & (1 << 2);  
       if (bit==4096>>10) c12 = c12 + frac19;
    bit = merge_byte & (1 << 1);  
       if (bit==4096>>11) c12 = c12 + frac20;
    bit = merge_byte;  
       if (bit==4096>>12) c12 = c12 + frac21;
    }
  return c12;
  }




void Read_Coefficients(unsigned int Coeff_Number, unsigned char* coefficients_){ 
    static unsigned char s[8]={0};
    static unsigned char r[8]={0};
    static unsigned char coefficients[8]={0};
    s[0]=CMD_a0h;
    s[1]=CMD_a0l   
    s[2]=CMD_b1h   
    s[3]=CMD_b1l   
    s[4]=CMD_b2h   
    s[5]=CMD_b2l  
    s[6]=CMD_c12h  
    s[7]=CMD_c12l  
    P1_4 = LOW;
    unsigned int Coeff_idx;
    for (Coeff_idx = 0; Coeff_idx < Coeff_Number; Coeff_idx++) {
    U1DBUF = s[Coeff_idx];
    while(U1ACTIVE);
    asm("NOP");	// Minor delay of 1 system clock (32MHz)
    U1DBUF =DUMMY; 
    while ( U1CSR & 0x01 ); // Wait for U1ACTIVE to be de-asserted (end of byte transfer)
    coefficients[Coeff_idx] = U1DBUF; 
    r[Coeff_idx] =coefficients[Coeff_idx];
    }
   U1DBUF =DUMMY; 
   while ( U1CSR & 0x01 ); // Wait for U1ACTIVE to be de-asserted (end of byte transfer)
   P1_4 = HIGH;
   a0h = r[0];
   a0l = r[1];
   b1h = r[2];
   b1l = r[3];
   b2h = r[4];
   b2l = r[5];
   c12h = r[6];
   c12l = r[7];
   
   memset (r,0,8);
   memset (s,0,8);
   memset (coefficients,0,8);
   
   a0 = a0_comp(a0h,a0l);
   b1 = b1_comp(b1h,b1l);
   b2 = b2_comp(b2h,b2l);
   c12 = c12_comp(c12h,c12l);
   
  coefficients_[0]=HIB(a0);
  coefficients_[1]=M2B(a0);
  coefficients_[2]=M1B(a0);
  coefficients_[3]=LOB(a0);
  coefficients_[4]=HIB(b1);
  coefficients_[5]=M2B(b1);
  coefficients_[6]=M1B(b1);
  coefficients_[7]=LOB(b1);
  coefficients_[8]=HIB(b2);
  coefficients_[9]=M2B(b2);
  coefficients_[10]=M1B(b2);
  coefficients_[11]=LOB(b2);
  coefficients_[12]=HIB(c12);
  coefficients_[13]=M2B(c12);
  coefficients_[14]=M1B(c12);
  coefficients_[15]=LOB(c12);
  } 



void Meas_Init(void){ 
  static unsigned char s[2]={0};
  P1_4 = LOW;
  s[0]=CMD_MEASURE;
  s[1]=DUMMY;
  SPI1_Send(s,2);
  P1_4 = HIGH;
  memset (s,0,2);
}

  void Read_Pressure(unsigned char* pressure){ 
      static unsigned char s[4]={0};
      static unsigned char r[4]={0};
      static unsigned char coefficients[4]={0};
      s[0]=CMD_Ph;
      s[1]=CMD_Pl;   
      s[2]=CMD_Th;   
      s[3]=CMD_Tl;   
      P1_4 = LOW;
      for (int Meas_idx = 0; Meas_idx < 4; Meas_idx++) {
      U1DBUF = s[Meas_idx];
      while(U1ACTIVE);
      asm("NOP");	// Minor delay of 1 system clock (32MHz)
      U1DBUF =DUMMY; 
      while ( U1CSR & 0x01 ); // Wait for U1ACTIVE to be de-asserted (end of byte transfer)
      coefficients[Meas_idx] = U1DBUF; 
      r[Meas_idx] =coefficients[Meas_idx];
      }
     U1DBUF =DUMMY; 
     while ( U1CSR & 0x01 ); // Wait for U1ACTIVE to be de-asserted (end of byte transfer)
     P1_4 = HIGH;
     Ph = r[0];
     Pl = r[1];
     Th = r[2];
     Tl = r[3];
     memcpy(pressure,r,4);
     memset (r,0,4);
     memset (s,0,4);
     memset (coefficients,0,4);
  } 