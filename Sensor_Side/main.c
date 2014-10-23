/***********************************************************************************
Filename:  CC2530 Wireless Firmware Given Imaging Capsule
Version:   1.0 
Author:    Beccani Marco
***********************************************************************************/

#include "hal_mcu.h"
#include "clock.h"
#include "timer1.h"
#include "stdio.h"
#include "stdlib.h"
#include "cc253x.h"
#include "string.h"
#include "hal_rf.h"
#include "basic_rf.h"
#include "hal_rf.h"
#include "rf_pack.h"
#include "adc.h"
#include "pwr_mode_manager.h"
#include "ads8320.h"
#include "spi_manager.h"
#include "lis331dlh.h"
#include "mpl115a.h"

#define DUMMY     0x00;

//  Check if can be removed
#define LOB(x) (*((unsigned char *)&(x)))
#define M1B(x) (*((unsigned char *)&(x)+1))
#define M2B(x) (*((unsigned char *)&(x)+2))
#define HIB(x) (*((unsigned char *)&(x)+3))

// Application parameters
#define RF_CHANNEL                5      // 2.4 GHz RF channel

// BasicRF address definitions
#define PAN_ID               0x2005
#define PC_ADDR              0xACC0
#define SENSOR_ADDR          0x0CCA

#define APP_PAYLOAD_LENGTH   30

// BasicRF Commands 
#define CONNECT                           'R'
#define ACK_RECEIVER                      'C'
#define ACK_WD                            'D'
#define ACK_RECEIVER2                     'c'
#define WD_PKG_DETECTED                   'k'
#define SLEEP_MESSAGE                     'S'    
#define SLEEP_ACK                         's'
#define WD_START_REQ                      'q'
#define ENABLE_ACQUISITION                'E'    
#define ENABLE_ACQUISITION_ACK            'e'
#define CONTINUE                          'x'
#define HOLD_ON                           'h'
#define PAUSE                             'J'

// Device Status 
#define WAIT_CONNECT           0x00
#define WD_CONNECTED           0x01
#define ALMOUST_READY          0x02
#define READY                  0x03
#define SLEEP                  0x04
#define ENABLE_ACQ             0x05

unsigned char __xdata SPI1TxBuf[256];
unsigned char __xdata SPI1RxBuf[256];

unsigned char __xdata SPI1TxIndex;
unsigned char __xdata SPI1RxIndex;

static uint8 pTxData[APP_PAYLOAD_LENGTH];
static uint8 pRxData[APP_PAYLOAD_LENGTH];
static basicRfCfg_t basicRfConfig;
static RF_PACKAGE_t data;
bool RF_Timeout_Status = 0;
unsigned int retry_counter =0; 

unsigned char device_status = 0x00;
unsigned char valid_package = 0x00;
unsigned int battery= 0;
unsigned long counter_loop = 0;

unsigned char ADC1_VAL[2]={0};
unsigned char ADC2_VAL[2]={0};
unsigned char ADC3_VAL[2]={0};
unsigned char ADC4_VAL[2]={0};
unsigned char ADC5_VAL[2]={0};
unsigned char ADC6_VAL[2]={0};

unsigned char acc[6] = {0};
unsigned char coefficients_stored[16] = {0};

bool  RF_Timeout(uint16 us_to_wait);

int main(void){  
    //halMcuInit();   // Set Main Clock source to XOSC AND RCOSC
    clock_init();
    EA = 1;         // Global interrupt enable
    device_status = WAIT_CONNECT;
    
    // Config SPI
    SPI1_Init();
    
    SPI1_Switch_SSN(4,HIGH);
    SPI1_Switch_SSN(3,HIGH);
    SPI1_Switch_SSN(2,HIGH);
    SPI1_Switch_SSN(1,HIGH);
    SPI1_Switch_SSN(0,HIGH);
    
    /*// Access Accellerometer
    Accell_Init();
    while (!Accell_Who_I_AM());  // Chech if the Accellerometer is connected*/
    
    for(int i=0; i<15; i++ ) coefficients_stored[i] =i;
    
    // Config basicRF
    
    basicRfConfig.panId = PAN_ID;
    basicRfConfig.channel = RF_CHANNEL;
    basicRfConfig.ackRequest = FALSE;
      
    // Initialize BasicRFpRF_PACKAGE
    basicRfConfig.myAddr = SENSOR_ADDR;
    basicRfInit(&basicRfConfig);
    basicRfReceiveOn(); 
    
    // Clear the Structure ... 
    RF_Package_Init(&data);
    data.PAYLOAD_DATA_SENSOR[0]=CONNECT; 
    RF_Package_Compose(&data);
    
    // COPY OF THE PACKAGE 
    memcpy(pTxData,data.PAYLOAD_DATA,APP_PAYLOAD_LENGTH);
    
    // MAX TX PWR
    TXPOWER = 0xF5;
    SPI1_Switch_SSN(2,LOW);
    
    // SLEEP TMR 
    SleepTimerInit(15);
    halMcuSetLowPowerMode(2);
    
    while(1){
      
    // WAIT CONNECT 
    while(device_status == WAIT_CONNECT ) {  
    
    // Send The package   
    basicRfSendPacket(PC_ADDR, pTxData, APP_PAYLOAD_LENGTH);
    
      RF_Timeout_Status = RF_Timeout(5000); 
      if ( (basicRfReceive(pRxData, APP_PAYLOAD_LENGTH,NULL)  >0) && (RF_Timeout_Status == FALSE)  ) {
        if ( (pRxData[0] == 0x01) &&  (pRxData[29] == 0x04)  && (pRxData[11] == ACK_RECEIVER) ){  
          valid_package = 0x01; 
          RF_Timeout_Status = TRUE;
          memset(pRxData,0,APP_PAYLOAD_LENGTH);
          memset(pTxData,0,APP_PAYLOAD_LENGTH);
          // Prepare the structure for the next status
          RF_Package_Init(&data); 
          data.PAYLOAD_DATA_SENSOR[0]=ACK_WD;
          RF_Package_Compose(&data);
          memcpy(pTxData,data.PAYLOAD_DATA,APP_PAYLOAD_LENGTH);
           // Move the device to the new status
          device_status = WD_CONNECTED;
        }
      }
      else {
        valid_package = 0x00;
        device_status = WAIT_CONNECT;
        SleepTimerInit(15);
        halMcuSetLowPowerMode(2);
      }
    }

    // WAIT CONNECTED
    while (device_status == WD_CONNECTED ) { 
      
    basicRfSendPacket(PC_ADDR, pTxData, APP_PAYLOAD_LENGTH); 
    
    RF_Timeout_Status = RF_Timeout(5000);
      if ( (basicRfReceive(pRxData, APP_PAYLOAD_LENGTH,NULL)  >0) && (RF_Timeout_Status == FALSE)  ) {
        if ( (pRxData[0] == 0x01) &&  (pRxData[29] == 0x04)  && (pRxData[11] == ACK_RECEIVER2)) {   
         valid_package = 0x01; 
         RF_Timeout_Status = TRUE;
         memset(pRxData,0,APP_PAYLOAD_LENGTH);
         memset(pTxData,0,APP_PAYLOAD_LENGTH); 
         // Prepare the structure for the next status
         RF_Package_Init(&data); 
         data.PAYLOAD_DATA_SENSOR[0]=WD_PKG_DETECTED;
         RF_Package_Compose(&data);
         memcpy(pTxData,data.PAYLOAD_DATA,APP_PAYLOAD_LENGTH);
         device_status = ALMOUST_READY;
         }
       }
       else {
        valid_package = 0x00;
        device_status = WD_CONNECTED;
        retry_counter = retry_counter + 1;
       if (retry_counter == 200) goto dev_sleep;
       }
    }
    
     // ALMOUST READY
     while (device_status == ALMOUST_READY ) { 
       
     RF_Timeout_Status = RF_Timeout(5000);
      if (( basicRfReceive(pRxData, APP_PAYLOAD_LENGTH,NULL)  >0 ) && (RF_Timeout_Status == FALSE))  {
          if ( (pRxData[0] == 0x01) &&  (pRxData[29] == 0x04)  && (pRxData[11] == WD_START_REQ))  {
            valid_package = 0x01; 
            RF_Timeout_Status = TRUE;
            // Send The package 
            basicRfSendPacket(PC_ADDR, pTxData, APP_PAYLOAD_LENGTH); 
            memset(pTxData,0,APP_PAYLOAD_LENGTH); 
            memset(pRxData,0,APP_PAYLOAD_LENGTH);
            // Prepare the structure for the next status
            RF_Package_Init(&data);   
            device_status = READY;
            retry_counter =0;
          }
      }
      else {
          valid_package = 0x00;
          device_status = ALMOUST_READY;
          retry_counter = retry_counter + 1;
          if (retry_counter == 1000) goto dev_sleep;
      }
     }
     
     // READY
     while (device_status == READY) { 
       RF_Timeout_Status = RF_Timeout(5000);
        // SLEEP
       if (( basicRfReceive(pRxData, APP_PAYLOAD_LENGTH,NULL)  >0 ) && (RF_Timeout_Status == FALSE))  {
          if ( (pRxData[0] == 0x01) &&  (pRxData[29] == 0x04)  && (pRxData[11] == SLEEP_MESSAGE)) {  
            valid_package = 0x01; 
            RF_Timeout_Status = TRUE;
            memset(pRxData,0,APP_PAYLOAD_LENGTH);
            data.PAYLOAD_DATA_SENSOR[0]=SLEEP_ACK;  
            RF_Package_Compose(&data);
            // COPY OF THE PACKAGE 
            memcpy(pTxData,data.PAYLOAD_DATA,APP_PAYLOAD_LENGTH);
            // Send The package 
            basicRfSendPacket(PC_ADDR, pTxData, APP_PAYLOAD_LENGTH); 
            device_status = SLEEP;
            retry_counter =0;
            }             
              
        if ( (pRxData[0] == 0x01) &&  (pRxData[29] == 0x04)  && (pRxData[11] == ENABLE_ACQUISITION )  ){  
            valid_package = 0x01; 
            RF_Timeout_Status = TRUE;
            SPI1_Switch_SSN(2,HIGH);
            memset(pRxData,0,APP_PAYLOAD_LENGTH); 
            memcpy(data.PAYLOAD_DATA_SENSOR,coefficients_stored,16);
            RF_Package_Compose(&data);
            // COPY OF THE PACKAGE 
            memcpy(pTxData,data.PAYLOAD_DATA,APP_PAYLOAD_LENGTH);
            // Send The package 
            basicRfSendPacket(PC_ADDR, pTxData, APP_PAYLOAD_LENGTH); 
            RF_Package_Init(&data);
            halMcuWaitUs(2000);
            device_status = ENABLE_ACQ;
            retry_counter =0;
        }       
        // Insert Here all the device Mode
          
        }
       
     else {
      valid_package = 0x00;
      device_status = READY;
      retry_counter = retry_counter + 1;
      if (retry_counter == 400) goto dev_sleep;
      }
      
     }
    
   // RUN
     while (device_status == ENABLE_ACQ ) {  
       counter_loop = counter_loop +1; 
       data.COUNTER_PKG[0] = counter_loop; 
       data.COUNTER_PKG[1] = counter_loop >> 8; 
       data.COUNTER_PKG[2] = counter_loop >> 16; 
       data.COUNTER_PKG[3] = counter_loop >> 24;
     
       adc_convert(ADC_REF_AVDD, ADC_12_BIT, ADC_AIN6,data.BATTERY_LEVEL);
     
       battery = data.BATTERY_LEVEL[1];
       battery = battery <<8;
       battery = battery +  data.BATTERY_LEVEL[0];
       battery = battery >> 4;
     
       if (battery <= 1210) goto dev_sleep;
       
        // On P1_0
       
        Read_accelleration(acc);
      
       APCFG = 127;
       
       adc_convert(ADC_REF_AVDD, ADC_12_BIT, ADC_AIN5,ADC6_VAL);   // P0_5
      
      
       adc_convert(ADC_REF_AVDD, ADC_12_BIT, ADC_AIN4,ADC5_VAL);   // P0_4
      
      
       adc_convert(ADC_REF_AVDD, ADC_12_BIT, ADC_AIN3,ADC4_VAL);   // P0_3
      

      
       adc_convert(ADC_REF_AVDD, ADC_12_BIT, ADC_AIN2,ADC3_VAL);   // P0_2
      

     
       adc_convert(ADC_REF_AVDD, ADC_12_BIT, ADC_AIN1,ADC2_VAL);   // P0_1
      

       adc_convert(ADC_REF_AVDD, ADC_12_BIT, ADC_AIN0,ADC1_VAL);   // P0_0
      
      
       APCFG = 0;
     
       data.RSSI_data = basicRfGetRssi();  
     

       memcpy(data.PAYLOAD_DATA_SENSOR,ADC1_VAL,2);
       memcpy(data.PAYLOAD_DATA_SENSOR+2,ADC2_VAL,2);
       memcpy(data.PAYLOAD_DATA_SENSOR+4,ADC3_VAL,2);
       memcpy(data.PAYLOAD_DATA_SENSOR+6,ADC4_VAL,2);
       memcpy(data.PAYLOAD_DATA_SENSOR+8,ADC5_VAL,2);
       memcpy(data.PAYLOAD_DATA_SENSOR+10,ADC6_VAL,2);
       memcpy(data.PAYLOAD_DATA_SENSOR+12,acc,6);
     
       toc(T1TICK_8);
     
       reset_TMR1(data.TIME_STAMP,sizeof(data.TIME_STAMP));
       
       RF_Package_Compose(&data);
       
       memcpy(pTxData,data.PAYLOAD_DATA,APP_PAYLOAD_LENGTH);
       RF_Package_Init(&data);
       tic(T1TICK_8);
  
       basicRfSendPacket(PC_ADDR, pTxData, APP_PAYLOAD_LENGTH);
       memset(pTxData,0,APP_PAYLOAD_LENGTH); 
       
       memset(acc,0,6);
       memset(ADC1_VAL,0,6);
       memset(ADC2_VAL,0,6);
       memset(ADC3_VAL,0,6);
       memset(ADC4_VAL,0,6);
       memset(ADC5_VAL,0,6);
       memset(ADC6_VAL,0,6);
     
       retry:
     
       RF_Timeout_Status = RF_Timeout(8000);
       if (( basicRfReceive(pRxData, APP_PAYLOAD_LENGTH,NULL)  >0 ) && (RF_Timeout_Status == FALSE))  {
         if ( (pRxData[0] == 0x01) &&  (pRxData[29] == 0x04)  && (pRxData[11] == CONTINUE)) {  
         valid_package = 0x01; 
         RF_Timeout_Status = TRUE;
         memset(pRxData,0,APP_PAYLOAD_LENGTH);
         device_status = ENABLE_ACQ;
         //halMcuWaitUs(25);           
         } // End Continue Case
       
       else if (pRxData[11] == HOLD_ON) {
       valid_package = 0x01; 
       RF_Timeout_Status = TRUE;
       memset(pRxData,0,APP_PAYLOAD_LENGTH);
       device_status = SLEEP;
       }
       
       else if (pRxData[11] == PAUSE) {
       valid_package = 0x01; 
       RF_Timeout_Status = TRUE;
       memset(pRxData,0,APP_PAYLOAD_LENGTH);
       device_status = READY;
       }
       
     } // End Receive
     
     else { 
       retry_counter = retry_counter + 1;
       if (retry_counter == 200) goto dev_sleep;
       else goto retry;
     }
     
     battery =0;
     retry_counter =0;
   }
   
    // SLEEP
    while (device_status == SLEEP ) {  
    dev_sleep:
    halMcuReset();
    }
    
    }
  }   // End Main      


/*********************************************************/

bool  RF_Timeout(uint16 us_to_wait){
  bool Timeout_Expired = FALSE;  
  while (!basicRfPacketIsReady()) {
    us_to_wait = us_to_wait - 1;
    halMcuWaitUs(1);
    if (us_to_wait == 0 ) {
    Timeout_Expired = TRUE; 
    break;
    }
  }
return  Timeout_Expired; 
}
   



  

  

  


