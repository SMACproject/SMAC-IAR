/******************************************************************************
    Filename: rf_pack.h 
    Structure Of the Wirelles Device
    Beccani Marco
    1.0 1/27/2013
******************************************************************************/
#ifndef RF_PACK_H
#define RF_PACK_H

#include "hal_types.h"
#include "hal_defs.h"
/*******************************************************************************/

#define COUNTER_PKG_SIZE   4
#define TIME_STAMP_SIZE    2
#define BATTERY_LEVEL_SIZE 2
#define DATA_SENSOR_SIZE  18
#define PAYLOAD_DATA_SIZE 30
/*******************************************************************************/


#define PAYLOAD_HEADER_VAL      0x01
#define PAYLOAD_END_VAL         0x04

/*******************************************************************************/

typedef struct {
unsigned char PAYLOAD_HEADER;   // Package Header 
unsigned char PAYLOAD_END;      // Package END 
unsigned char COUNTER_PKG[COUNTER_PKG_SIZE];   /* Package Counter is increased every time a package is transmitted 
                                                *  COUNTER_PKG[3] is filled with bits 31-25 of the counter 
                                                *  COUNTER_PKG[2] is filled with bits 24-17 of the counter 
                                                *  COUNTER_PKG[1] is filled with bits 16-9 of the counter 
                                                *  COUNTER_PKG[0] is filled with bits  8-1 of the counter  */ 

unsigned char  TIME_STAMP[TIME_STAMP_SIZE];    /* TIME_STAMP[1] is filled with bits  16-9 of the time stamp chronometer 
                                               *  TIME_STAMP[0] is filled with bits   8-1 of the time stamp chronometer  */

unsigned char  BATTERY_LEVEL[BATTERY_LEVEL_SIZE]; /* BATTERY_LEVEL[1] is filled with bits  16-9 of the battery level 
                                                  *  BATTERY_LEVEL[0] is filled with bits   8-1 of the battery level */

unsigned char  PAYLOAD_DATA_SENSOR[DATA_SENSOR_SIZE];

unsigned char  PAYLOAD_DATA[PAYLOAD_DATA_SIZE];

unsigned char  PAYLOAD_SIZE;

unsigned char  RSSI_data;

} RF_PACKAGE_t;

/***********************************************************************************
* GLOBAL FUNCTIONS
***********************************************************************************/

bool RF_Package_Init(RF_PACKAGE_t* pRF_PACKAGE);
bool RF_Package_Compose(RF_PACKAGE_t* pRF_PACKAGE);

#endif
