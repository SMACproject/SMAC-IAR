#ifndef HAL_DMA_H
#define HAL_DMA_H

#include "hal_board.h"
#include "hal_types.h"

#define DMA_CHANNEL_0  0x01
#define DMA_CHANNEL_1  0x02
#define DMA_CHANNEL_2  0x04
#define DMA_CHANNEL_3  0x08
#define DMA_CHANNEL_4  0x10

#define VLEN_USE_LEN            0x00 // Use LEN for transfer count
#define VLEN_FIXED              0x00 // Use LEN for transfer count
#define VLEN_1_P_VALOFFIRST     0x01 // Transfer the first byte + the number of bytes indicated by the first byte
#define VLEN_VALOFFIRST         0x02 // Transfer the number of bytes indicated by the first byte (starting with the first byte)
#define VLEN_1_P_VALOFFIRST_P_1 0x03 // Transfer the first byte + the number of bytes indicated by the first byte + 1 more byte
#define VLEN_1_P_VALOFFIRST_P_2 0x04 // Transfer the first byte + the number of bytes indicated by the first byte + 2 more bytes

#define WORDSIZE_BYTE           0x00 // Transfer a byte at a time
#define WORDSIZE_WORD           0x01 // Transfer a 16-bit word at a time

#define TMODE_SINGLE            0x00 // Transfer a single byte/word after each DMA trigger
#define TMODE_BLOCK             0x01 // Transfer block of data (length len) after each DMA trigger
#define TMODE_SINGLE_REPEATED   0x02 // Transfer single byte/word (after len transfers, rearm DMA)
#define TMODE_BLOCK_REPEATED    0x03 // Transfer block of data (after len transfers, rearm DMA)

#define DMATRIG_NONE           0   // No trigger, setting DMAREQ.DMAREQx bit starts transfer
#define DMATRIG_PREV           1   // DMA channel is triggered by completion of previous channel
#define DMATRIG_T1_CH0         2   // Timer 1, compare, channel 0
#define DMATRIG_T1_CH1         3   // Timer 1, compare, channel 1
#define DMATRIG_T1_CH2         4   // Timer 1, compare, channel 2
#define DMATRIG_T2_COMP        5   // Timer 2, compare
#define DMATRIG_T2_OVFL        6   // Timer 2, overflow
#define DMATRIG_T3_CH0         7   // Timer 3, compare, channel 0
#define DMATRIG_T3_CH1         8   // Timer 3, compare, channel 1
#define DMATRIG_T4_CH0         9   // Timer 4, compare, channel 0
#define DMATRIG_T4_CH1        10   // Timer 4, compare, channel 1
#define DMATRIG_ST            11   // Sleep Timer compare
#define DMATRIG_IOC_0         12   // Port 0 I/O pin input transition
#define DMATRIG_IOC_1         13   // Port 1 I/O pin input transition
#define DMATRIG_URX0          14   // USART0 RX complete
#define DMATRIG_UTX0          15   // USART0 TX complete
#define DMATRIG_URX1          16   // USART1 RX complete
#define DMATRIG_UTX1          17   // USART1 TX complete
#define DMATRIG_FLASH         18   // Flash data write complete
#define DMATRIG_RADIO         19   // RF packet byte received/transmit
#define DMATRIG_ADC_CHALL     20   // ADC end of a conversion in a sequence, sample ready
#define DMATRIG_ADC_CH0       21   // ADC end of conversion channel 0 in sequence, sample ready
#define DMATRIG_ADC_CH1       22   // ADC end of conversion channel 1 in sequence, sample ready
#define DMATRIG_ADC_CH2       23   // ADC end of conversion channel 2 in sequence, sample ready
#define DMATRIG_ADC_CH3       24   // ADC end of conversion channel 3 in sequence, sample ready
#define DMATRIG_ADC_CH4       25   // ADC end of conversion channel 4 in sequence, sample ready
#define DMATRIG_ADC_CH5       26   // ADC end of conversion channel 5 in sequence, sample ready
#define DMATRIG_ADC_CH6       27   // ADC end of conversion channel 6 in sequence, sample ready
#define DMATRIG_ADC_CH7       28   // ADC end of conversion channel 7 in sequence, sample ready
#define DMATRIG_ENC_DW        29   // AES encryption processor requests download input data
#define DMATRIG_ENC_UP        30   // AES encryption processor requests upload output data

#define SRCINC_0         0x00 // Increment source pointer by 0 bytes/words after each transfer
#define SRCINC_1         0x01 // Increment source pointer by 1 bytes/words after each transfer
#define SRCINC_2         0x02 // Increment source pointer by 2 bytes/words after each transfer
#define SRCINC_M1        0x03 // Decrement source pointer by 1 bytes/words after each transfer

#define DESTINC_0        0x00 // Increment destination pointer by 0 bytes/words after each transfer
#define DESTINC_1        0x01 // Increment destination pointer by 1 bytes/words after each transfer
#define DESTINC_2        0x02 // Increment destination pointer by 2 bytes/words after each transfer
#define DESTINC_M1       0x03 // Decrement destination pointer by 1 bytes/words after each transfer

#define IRQMASK_DISABLE  0x00 // Disable interrupt generation
#define IRQMASK_ENABLE   0x01 // Enable interrupt generation upon DMA channel done

#define M8_USE_8_BITS    0x00 // Use all 8 bits for transfer count
#define M8_USE_7_BITS    0x01 // Use 7 LSB for transfer count

#define PRI_LOW          0x00 // Low, CPU has priority
#define PRI_GUARANTEED   0x01 // Guaranteed, DMA at least every second try
#define PRI_HIGH         0x02 // High, DMA has priority
#define PRI_ABSOLUTE     0x03 // Highest, DMA has priority. Reserved for DMA port access.


#pragma bitfields=reversed
typedef struct {
   BYTE SRCADDRH;
   BYTE SRCADDRL;
   BYTE DESTADDRH;
   BYTE DESTADDRL;
   BYTE VLEN      : 3;
   BYTE LENH      : 5;
   BYTE LENL      : 8;
   BYTE WORDSIZE  : 1;
   BYTE TMODE     : 2;
   BYTE TRIG      : 5;
   BYTE SRCINC    : 2;
   BYTE DESTINC   : 2;
   BYTE IRQMASK   : 1;
   BYTE M8        : 1;
   BYTE PRIORITY  : 2;
} halDMADesc_t;
#pragma bitfields=default

/*********************************************************************
 * GLOBAL VARIABLES
 */

//extern halDMADesc_t dmaCh0;
//extern halDMADesc_t dmaCh1234[4];

#define DMA_SET_ADDR_DESC0(a)           \
   do{                                  \
      DMA0CFGH = (BYTE)( (WORD)a >> 8 );\
      DMA0CFGL = (BYTE)( (WORD)a );     \
   } while(0)


#define DMA_SET_ADDR_DESC1234(a)        \
   do{                                  \
      DMA1CFGH = (BYTE)( (WORD)a >> 8 );\
      DMA1CFGL = (BYTE)( (WORD)a );     \
   } while(0)

#define DMA_ARM_CHANNEL(ch)           \
   do{                                \
      DMAARM = ((0x01 << ch) & 0x1F); \
   } while(0)

#define DMA_ABORT_CHANNEL(ch)    DMAARM = (0x80 | ((0x01 << ch) & 0x1F))
#define DMA_MAN_TRIGGER(ch)      DMAREQ = (0x01 << ch)
#define DMA_START_CHANNEL(ch)    DMA_MAN_TRIGGER(ch)

// Macro for quickly setting the destination address of a DMA structure
#define SET_DMA_DEST(pDmaDesc, dest)                 \
   do{                                               \
      pDmaDesc->DESTADDRH = (BYTE) ((WORD)dest >> 8);\
      pDmaDesc->DESTADDRL = (BYTE)  (WORD)dest;      \
   } while (0);

// Macro for quickly setting the source address of a DMA structure
#define SET_DMA_SOURCE(pDmaDesc, source)              \
   do{                                                \
      pDmaDesc->SRCADDRH = (BYTE) ((WORD)source >> 8);\
      pDmaDesc->SRCADDRL = (BYTE)  (WORD)source;      \
   } while (0)

// Macro for quickly setting the number of bytes to be transferred by the DMA.
// max lenght is 0x1FFF
#define SET_DMA_LENGTH(pDmaDesc, length)          \
   do{                                            \
      pDmaDesc->LENH = (BYTE) ((WORD)length >> 8);\
      pDmaDesc->LENL = (BYTE)  (WORD)length;      \
   } while (0)

// Macro for getting the destination address of a DMA channel
#define GET_DMA_DEST(pDmaDesc)   \
   ( (WORD)pDmaDesc->DESTADDRL | ( (WORD)pDmaDesc->DESTADDRH << 8 ))

// Macro for getting the source address of a DMA channel
#define GET_DMA_SOURCE(pDmaDesc) \
   ( (WORD)pDmaDesc->SRCADDRL  | ( (WORD)pDmaDesc->SRCADDRH << 8 ))


#endif

