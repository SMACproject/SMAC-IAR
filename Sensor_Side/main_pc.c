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
#include "dma.h"
#include "pwr_mode_manager.h"
#include "uart_manager.h"

#define DUMMY     0x00;
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

// RF Commands 
#define CONNECT              'R'
#define ACK_RECEIVER         'C'
#define ACK_WD               'D'
#define ACK_RECEIVER2        'c'
#define BRIDGE_CONNECTED     'p'
#define WD_PKG_DETECTED      'k'
#define CONTINUE             'x'
#define PAUSE                'J'

// Others Commands
#define WAIT_CONN     'W'
#define OK_CONN       'w'

// Device Status 
#define WAIT_PC                0x10
#define WAIT_CONNECT           0x00
#define WD_CONNECTED           0x01
#define READY                  0x02
#define ENABLE_ACQ             0x30
#define SLEEP_STATUS           0x40
//#define RUN_BABY               0x50


BOOL __xdata DMA_COMPLETE = FALSE;
_Pragma("vector=0x43") __near_func __interrupt void DMA_COMPLETE_ISR(void);

unsigned char uart_status; 
unsigned char uart_CMD[1]; 

static uint8 pTxData[APP_PAYLOAD_LENGTH];
static uint8 pRxData[APP_PAYLOAD_LENGTH];
static basicRfCfg_t basicRfConfig;

static RF_PACKAGE_t data; // Pointer To The Structure Where the MCU DATA are Saved
 
unsigned char device_status = WAIT_CONNECT ;
unsigned char valid_package = 0x00;

bool RF_Timeout_Status = 0;

unsigned char data_uart[30];
unsigned char coefficients_stored[16];
bool RF_Timeout(uint16 us_to_wait);

bool HOLD_try = FALSE;

int main(void) { 

    //halMcuInit();   // Set Main Clock source to XOSC AND RCOSC
    clock_init();
    EA = 1; 
    
    
    // Config DMA 
   halDMADesc_t dmaChannel;
   DMAIE = 0x01;
   
   dmaChannel.SRCADDRH = (uint8)( (uint16)(&data_uart) >> 8 ); 
   dmaChannel.SRCADDRL = (uint8)( (uint16)(&data_uart) & 0xFF );  
   dmaChannel.DESTADDRH = (uint8)( (uint16)(&X_U0DBUF) >> 8 ); 
   dmaChannel.DESTADDRL = (uint8)( (uint16)(&X_U0DBUF) & 0xFF );  
   
   SET_WORD(dmaChannel.LENH, dmaChannel.LENL, sizeof(pRxData));    // Setting the number of bytes to transfer.
   dmaChannel.VLEN      = VLEN_USE_LEN;  // Using the length field to determine how many bytes to transfer.
   dmaChannel.PRIORITY  = PRI_HIGH;      // High priority.
   dmaChannel.M8        = M8_USE_8_BITS; // Irrelevant since length is determined by the LENH and LENL.
   dmaChannel.IRQMASK   = TRUE;         // The DMA shall not issue an IRQ upon completion.
   dmaChannel.DESTINC   = DESTINC_0;     // The destination address is to be incremented by 1 after each transfer.
   dmaChannel.SRCINC    = SRCINC_1;      // The source address inremented by 1 byte after each transfer.
   dmaChannel.TRIG      = DMATRIG_UTX0;  
   dmaChannel.TMODE     = TMODE_SINGLE;   // The number of bytes specified by LENH and LENL is transferred.
   dmaChannel.WORDSIZE  = WORDSIZE_BYTE; // One byte is transferred each time.
      
    // Config basicRF
    
    basicRfConfig.panId = PAN_ID;
    basicRfConfig.channel = RF_CHANNEL;
    basicRfConfig.ackRequest = FALSE;
      
    // Initialize BasicRFpRF_PACKAGE
    basicRfConfig.myAddr = PC_ADDR;
    basicRfInit(&basicRfConfig);
    basicRfReceiveOn(); 
   
    TXPOWER = 0xF5;
    
    uart0_Init();
    
    RF_Package_Init(&data);
    RF_Package_Compose(&data);
    memcpy(pTxData,data.PAYLOAD_DATA,APP_PAYLOAD_LENGTH);
    
    
    hold_on:
     
      if( uart_status != START_SCANNING) {
      halMcuWaitMs(15);
    goto hold_on;}
    uart_status =0;
    
    
   DMA_SET_ADDR_DESC0(&dmaChannel);
   DMA0CFGH =       ( (WORD)&dmaChannel >> 8 );
   DMA0CFGL =       ( (WORD)&dmaChannel );     
   
   DMA_ARM_CHANNEL(0);
    
    while(1)
    {
      

      
      // Wait for the connection with the Wireless Device
      if (device_status == WAIT_CONNECT  )  {
         RF_Timeout_Status = RF_Timeout(5000); 
         if (( basicRfReceive(pRxData, APP_PAYLOAD_LENGTH,NULL)  >0 ) && (RF_Timeout_Status == FALSE)) {
          if ( (pRxData[0] == 0x01) && (pRxData[11] == CONNECT) &&  (pRxData[29] == 0x04)){  
            valid_package = 0x01;
            RF_Timeout_Status = TRUE;
            
            pTxData[11] = ACK_RECEIVER;
            basicRfSendPacket(SENSOR_ADDR, pTxData, APP_PAYLOAD_LENGTH);
            
            memset(pRxData,0,APP_PAYLOAD_LENGTH);            
            
            uart_CMD[0] = OK_CONN;
            uart0_Send(uart_CMD,sizeof(uart_CMD));
            
            device_status = WD_CONNECTED;
            }
          }
        else {
          valid_package = 0x00;
          device_status = WAIT_CONNECT;
          uart_CMD[0] = WAIT_CONN;
          uart0_Send(uart_CMD,sizeof(uart_CMD));
        }
      }
      
   // Wait for the  ACK connection with the Wireless Device
    if (device_status == WD_CONNECTED  )  {  
      
      RF_Timeout_Status = RF_Timeout(5000);
      if (( basicRfReceive(pRxData, APP_PAYLOAD_LENGTH,NULL)  >0 ) && (RF_Timeout_Status == FALSE)) {
          if ( (pRxData[0] == 0x01) && (pRxData[11] == ACK_WD) &&  (pRxData[29] == 0x04)    ){  
            valid_package = 0x01;
            RF_Timeout_Status = TRUE;
            
            pTxData[11] = ACK_RECEIVER2;
            basicRfSendPacket(SENSOR_ADDR, pTxData, APP_PAYLOAD_LENGTH); 
            memset(pRxData,0,APP_PAYLOAD_LENGTH);
            
            device_status = WAIT_PC;
            }
        }
        else {
          valid_package = 0x00;
          device_status = WD_CONNECTED;
        }
      }
      
      // Wait for the  ACK connection with the PC
      if (device_status == WAIT_PC  )  {
        
        if (uart_status == PC_CONNECTED) {
          uart_CMD[0] = BRIDGE_CONNECTED;
          uart0_Send(uart_CMD,sizeof(uart_CMD));
          device_status = WAIT_PC; 
          uart_status = 0; 
        }
        
        if (uart_status == WD_START_REQ) {
            pTxData[11] = WD_START_REQ;
            basicRfSendPacket(SENSOR_ADDR, pTxData, APP_PAYLOAD_LENGTH); 
            RF_Timeout_Status = RF_Timeout(5000);
            
            if (( basicRfReceive(pRxData, APP_PAYLOAD_LENGTH,NULL)  >0 ) && (RF_Timeout_Status == FALSE)) {
              if ( (pRxData[0] == 0x01) && (pRxData[11] == WD_PKG_DETECTED) &&  (pRxData[29] == 0x04)    ){  
                valid_package = 0x01;
                RF_Timeout_Status = TRUE;
                memset(pRxData,0,APP_PAYLOAD_LENGTH);
                
                uart_CMD[0] = WD_PKG_DETECTED;
                uart0_Send(uart_CMD,sizeof(uart_CMD));
                device_status = WAIT_PC;
                uart_status = 0;
                }  
            }
        }
        
       if (uart_status == DEV_SLEEP) {
            pTxData[11] = DEV_SLEEP;
            basicRfSendPacket(SENSOR_ADDR, pTxData, APP_PAYLOAD_LENGTH); 
            RF_Timeout_Status = RF_Timeout(5000);
            if (( basicRfReceive(pRxData, APP_PAYLOAD_LENGTH,NULL)  >0 ) && (RF_Timeout_Status == FALSE)) {
              if ( (pRxData[0] == 0x01) && (pRxData[11] == SLEEP_ACK) &&  (pRxData[29] == 0x04)    ){  
              valid_package = 0x01;
              RF_Timeout_Status = TRUE;
              memset(pRxData,0,APP_PAYLOAD_LENGTH);
              
              uart_CMD[0] = SLEEP_ACK;
              uart0_Send(uart_CMD,sizeof(uart_CMD));
              device_status = SLEEP_STATUS;
              uart_status = 0;
              }  
            }
        }
        
       if (uart_status == ENABLE_ACQUISITION) {
            pTxData[11] = ENABLE_ACQUISITION;
            basicRfSendPacket(SENSOR_ADDR, pTxData, APP_PAYLOAD_LENGTH); 
            RF_Timeout_Status = RF_Timeout(5000);
            if (( basicRfReceive(pRxData, APP_PAYLOAD_LENGTH,NULL)  >0 ) && (RF_Timeout_Status == FALSE)) {
              if ( (pRxData[0] == 0x01) &&  (pRxData[29] == 0x04) ){  
              valid_package = 0x01;
              RF_Timeout_Status = TRUE;
              uart_CMD[0] = ENABLE_ACQUISITION_ACK;
              uart0_Send(uart_CMD,1);
              uart0_Send(pRxData,APP_PAYLOAD_LENGTH); // Send the coefficients
              memset(pRxData,0,APP_PAYLOAD_LENGTH);
              device_status = ENABLE_ACQ;
              uart_status = CONTINUE;
              }  
            }
        }
      if (uart_status==0) NOP();
      }
     
      while (device_status == ENABLE_ACQ  )  { 
         
           RF_Timeout_Status = RF_Timeout(8000);
           if (( basicRfReceive(pRxData, APP_PAYLOAD_LENGTH, NULL)  >0 ) && (RF_Timeout_Status == FALSE)) {
            if ( (pRxData[0] == 0x01)  &&  (pRxData[29] == 0x04) &&  (uart_status == CONTINUE) ){  
            
            memcpy(data_uart,pRxData,APP_PAYLOAD_LENGTH);
            uart0_Send(data_uart,30);           
            
            pTxData[11] = CONTINUE;
            basicRfSendPacket(SENSOR_ADDR, pTxData, APP_PAYLOAD_LENGTH);
            device_status = ENABLE_ACQ;
            }
           }
          
           else {
             //memset(data_uart,0,APP_PAYLOAD_LENGTH);
             pTxData[11] = CONTINUE;
             basicRfSendPacket(SENSOR_ADDR, pTxData, APP_PAYLOAD_LENGTH);
             data_uart[0] = 0x01;
             data_uart[29] = 0x04;
             uart0_Send(data_uart,APP_PAYLOAD_LENGTH); 
           }
           
      if (uart_status == HOLD_ON) {
        
        RF_Timeout_Status = RF_Timeout(8000);
        if (( basicRfReceive(pRxData, APP_PAYLOAD_LENGTH, NULL)  >0 ) && (RF_Timeout_Status == FALSE)) {
             if ( (pRxData[0] == 0x01)  &&  (pRxData[29] == 0x04) ){
             
                    memcpy(data_uart,pRxData,APP_PAYLOAD_LENGTH);
                
                 DMAIRQ = 0x00;
                 DMA_START_CHANNEL(0);
                 halMcuWaitUs(5);
                
                while (!DMA_COMPLETE);   
                if(DMA_COMPLETE) {
                    DMA_ABORT_CHANNEL(0);
                    DMA_ARM_CHANNEL(0);
                    DMA_START_CHANNEL(0);
                    DMAIE = 0x01;
                 }
                
                pTxData[11] = HOLD_ON;
                if (HOLD_try == FALSE) {
                  basicRfSendPacket(SENSOR_ADDR, pTxData, APP_PAYLOAD_LENGTH);
                  halMcuWaitMs(2);
                  basicRfSendPacket(SENSOR_ADDR, pTxData, APP_PAYLOAD_LENGTH);
                  halMcuWaitMs(2);
                  basicRfSendPacket(SENSOR_ADDR, pTxData, APP_PAYLOAD_LENGTH);
                  device_status = WAIT_CONNECT;
                  uart_status = 0;
                  HOLD_try = TRUE;
                }
                
                memset(pRxData,0,APP_PAYLOAD_LENGTH);
                }
               
              }
           halMcuWaitMs(1000);
           halMcuReset();
             
           }
           
   
      memset(pRxData,0,APP_PAYLOAD_LENGTH);
      memset(data_uart,0,APP_PAYLOAD_LENGTH);   
      } 
        
        
       if (device_status == SLEEP_STATUS  )  
        { 
          NOP();
        } 
      
    } // End While 1 
              
}
// End Main      


  
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
  
  _Pragma("vector=0x43") __near_func __interrupt void DMA_COMPLETE_ISR(void){ 
  DMAIF = 0x00;  // Clear the Dma interrupt FLAG 
  DMAIRQ = 0x00; 
  DMA_COMPLETE = TRUE;   
}
  



