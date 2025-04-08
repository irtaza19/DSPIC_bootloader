/**
* @file        fifo.h
* @author      IRQdesign d.o.o 
* @date        January 2022
* @version     1.0.0
*/

/* Define to prevent recursive inclusion *********************************** */
#ifndef __FIFO_H
#define __FIFO_H
/* Includes **************************************************************** */
#include "stdint.h"

/* Module configuration **************************************************** */
#define FIFO_BUFFER_SIZE                                        512 

/* Exported constants ****************************************************** */

/* Exported macros ********************************************************* */
#define appendFIFO(fifo,data)                       fifo->buffer[fifo->widx++] = data;    \
                                                    if(fifo->widx >= FIFO_BUFFER_SIZE)   \
                                                    {                                   \
                                                        fifo->widx = 0;                   \
                                                    }                                   \
                                                                                          
#define flushFIFO(fifo)                             (fifo->ridx = fifo->widx)

/* Exported types ********************************************************** */
typedef enum 
{
    FIFO_SCAN_STATUS_CHAR_NOT_FOUND = 0x00,
    FIFO_SCAN_STATUS_CHAR_FOUND,
} FIFO_SCAN_STATUS;

typedef struct fifo_module_t
{
    uint8_t buffer[FIFO_BUFFER_SIZE];
    uint16_t widx;
    uint16_t ridx;
} FIFO;

/* Exported variables ****************************************************** */

/* Exported functions ****************************************************** */
uint16_t dataAvailableFIFO(FIFO * fifo);
uint8_t getByteFIFO(FIFO * fifo);
uint8_t scan4CharFIFO(FIFO * fifo,uint8_t c);

#endif 

