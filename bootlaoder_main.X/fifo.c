/* Includes **************************************************************** */
#include "fifo.h"
#include "debug.h"

/* Private types *********************************************************** */

/* Private constants ******************************************************* */

/* Private macros ********************************************************** */

/* Private variables ******************************************************* */

/* Private function prototypes ********************************************* */

/* Exported functions ****************************************************** */

uint16_t dataAvailableFIFO(FIFO * fifo)
{
    if(fifo->widx >= fifo->ridx)
    {
        return (fifo->widx - fifo->ridx);
    }
    else
    {
        return (FIFO_BUFFER_SIZE - fifo->ridx + fifo->widx );
    }
}

uint8_t getByteFIFO(FIFO * fifo)
{
    uint8_t data = fifo->buffer[fifo->ridx++];
    /* if(data == '\r') */
    /* { */
    /*     printDEBUG(DSYS | DERROR,"[\\r]\n"); */
    /* } */
    /* else if(data == '\n') */
    /* { */
    /*     printDEBUG(DSYS | DERROR,"[\\n]\n"); */
    /* } */
    /* else */
    /* { */
    /*     printDEBUG(DSYS | DERROR,"[%c]\n",data); */
    /* } */
    if(fifo->ridx >= FIFO_BUFFER_SIZE)
    {
        fifo->ridx = 0x00;
    }
    return data;
}

uint8_t scan4CharFIFO(FIFO * fifo,uint8_t c)
{
    uint8_t flag = FIFO_SCAN_STATUS_CHAR_NOT_FOUND;
    uint16_t ridx = fifo->ridx;
    while(ridx != fifo->widx)
    {
        uint8_t data = fifo->buffer[ridx++];
        if(c == data)
        {
            flag = FIFO_SCAN_STATUS_CHAR_FOUND;
            break;
        }
        if(ridx >= FIFO_BUFFER_SIZE)
        {
            ridx = 0x00;
        }
    }
    return flag;
}

/* Private functions ******************************************************* */

/* ***************************** END OF FILE ******************************* */


