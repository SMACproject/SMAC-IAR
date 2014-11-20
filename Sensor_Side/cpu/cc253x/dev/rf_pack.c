/******************************************************************************
    Filename: rf_pack.c 
    Structure Of the Wirelles Device
    Beccani Marco
    1.0 1/27/2013
******************************************************************************/
#include "hal_types.h"
#include "hal_defs.h"
#include "stdio.h"
#include "rf_pack.h"
#include "string.h"

/***********************************************************************************
* LOCAL FUNCTIONS
***********************************************************************************/

bool RF_Package_Init(RF_PACKAGE_t* pRF_PACKAGE)
{
    
    pRF_PACKAGE->PAYLOAD_HEADER = PAYLOAD_HEADER_VAL;
    memset(pRF_PACKAGE->COUNTER_PKG,0,sizeof(pRF_PACKAGE->COUNTER_PKG));  
    memset(pRF_PACKAGE->TIME_STAMP,0,sizeof(pRF_PACKAGE->TIME_STAMP));  
    memset(pRF_PACKAGE->BATTERY_LEVEL,0,sizeof(pRF_PACKAGE->BATTERY_LEVEL)); 
    memset(pRF_PACKAGE->PAYLOAD_DATA,0,sizeof(pRF_PACKAGE->PAYLOAD_DATA)); 
    memset(pRF_PACKAGE->PAYLOAD_DATA_SENSOR,0,sizeof(pRF_PACKAGE->PAYLOAD_DATA_SENSOR)); 
    pRF_PACKAGE->PAYLOAD_SIZE = PAYLOAD_DATA_SIZE;  
    pRF_PACKAGE->RSSI_data = 0;
    pRF_PACKAGE->PAYLOAD_END =PAYLOAD_END_VAL; 
    return TRUE;
}

/***********************************************************************************
* LOCAL FUNCTIONS
***********************************************************************************/

bool RF_Package_Compose(RF_PACKAGE_t* pRF_PACKAGE)
{ 
  memcpy(&pRF_PACKAGE->PAYLOAD_DATA, (const void *) &pRF_PACKAGE->PAYLOAD_HEADER, sizeof(pRF_PACKAGE->PAYLOAD_HEADER));
  memcpy(pRF_PACKAGE->PAYLOAD_DATA + sizeof(pRF_PACKAGE->PAYLOAD_HEADER), pRF_PACKAGE->COUNTER_PKG, sizeof(pRF_PACKAGE->COUNTER_PKG));
  memcpy(pRF_PACKAGE->PAYLOAD_DATA + sizeof(pRF_PACKAGE->PAYLOAD_HEADER) + sizeof(pRF_PACKAGE->COUNTER_PKG), pRF_PACKAGE->TIME_STAMP, sizeof(pRF_PACKAGE->TIME_STAMP));
  memcpy(pRF_PACKAGE->PAYLOAD_DATA + sizeof(pRF_PACKAGE->PAYLOAD_HEADER) + sizeof(pRF_PACKAGE->COUNTER_PKG) + sizeof(pRF_PACKAGE->TIME_STAMP), pRF_PACKAGE->BATTERY_LEVEL, sizeof(pRF_PACKAGE->BATTERY_LEVEL));	 
  memcpy(pRF_PACKAGE->PAYLOAD_DATA + sizeof(pRF_PACKAGE->PAYLOAD_HEADER) + sizeof(pRF_PACKAGE->COUNTER_PKG) + sizeof(pRF_PACKAGE->TIME_STAMP) + sizeof(pRF_PACKAGE->BATTERY_LEVEL), (const void *) &pRF_PACKAGE->PAYLOAD_SIZE, sizeof(pRF_PACKAGE->PAYLOAD_SIZE));
  memcpy(pRF_PACKAGE->PAYLOAD_DATA + sizeof(pRF_PACKAGE->PAYLOAD_HEADER) + sizeof(pRF_PACKAGE->COUNTER_PKG) + sizeof(pRF_PACKAGE->TIME_STAMP) + sizeof(pRF_PACKAGE->BATTERY_LEVEL)+ sizeof(pRF_PACKAGE->PAYLOAD_SIZE), (const void *) &pRF_PACKAGE->RSSI_data, sizeof(pRF_PACKAGE->RSSI_data));
  memcpy(pRF_PACKAGE->PAYLOAD_DATA + sizeof(pRF_PACKAGE->PAYLOAD_HEADER) + sizeof(pRF_PACKAGE->COUNTER_PKG) + sizeof(pRF_PACKAGE->TIME_STAMP) + sizeof(pRF_PACKAGE->BATTERY_LEVEL) + sizeof(pRF_PACKAGE->PAYLOAD_SIZE) + sizeof(pRF_PACKAGE->RSSI_data) , pRF_PACKAGE->PAYLOAD_DATA_SENSOR, sizeof(pRF_PACKAGE->PAYLOAD_DATA_SENSOR));
  memcpy(pRF_PACKAGE->PAYLOAD_DATA + sizeof(pRF_PACKAGE->PAYLOAD_HEADER) + sizeof(pRF_PACKAGE->COUNTER_PKG) + sizeof(pRF_PACKAGE->TIME_STAMP) + sizeof(pRF_PACKAGE->BATTERY_LEVEL)+ sizeof(pRF_PACKAGE->PAYLOAD_SIZE) + sizeof(pRF_PACKAGE->PAYLOAD_DATA_SENSOR) + 1, (const void *) &pRF_PACKAGE->PAYLOAD_END, sizeof(pRF_PACKAGE->PAYLOAD_END));
  return TRUE;
}
