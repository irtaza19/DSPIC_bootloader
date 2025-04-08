#ifndef __UART_H
#define	__UART_H

#include <xc.h> 
#include "fifo.h"


void initUART1(uint32_t baudrate);
void deinitUART1(void);
void PPS(void);
void txByteUART1(char send);
uint8_t txRegisterUART1Empty(void);
char rxByteUART1(void);
void startAutoBaudrateUART1(void);
void __attribute__((interrupt)) U1RXInterrupt( void );

extern volatile FIFO g_FIFO_USART1;
#endif
