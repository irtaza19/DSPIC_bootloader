/* 
 * File:   uart.c
 * Author: semir
 *
 * Created on June 16, 2022, 8:15 AM
 */

#include "uart.h"
#include "system.h"
#include "debug.h"
#include "fifo.h"

volatile FIFO g_FIFO_USART1;

void initUART1(uint32_t baudrate)
{
     //RX,TX
    //TRISCbits.TRISC9=1;
    //TRISCbits.TRISC8=0;

    
    TRISBbits.TRISB6=1;                                           //CHANGED BY MUNSIF AND AQIB ACCORDING TO THE PINS
    TRISBbits.TRISB5=0;
    
     //peripheral pin select
     
    // AD1PCFGL = 0xFFFF;
    AD1CON1bits.ADON = 0;
     //peripheral pin select
    PPS();
    
 
    U1MODEbits.STSEL = 0; // 1-Stop bit
    U1MODEbits.PDSEL = 0; // No Parity, 8-Data bits
    U1MODEbits.BRGH = 0; // Standard-Speed mode
    if(baudrate == 0)
    {
        U1MODEbits.ABAUD = 1; // Auto-Baud disabled
        baudrate = 9600;
        //baudrate *= 2;
        U1BRG=((FCY/baudrate)/16)-1;
    }
    else
    {
        U1MODEbits.ABAUD = 0; // Auto-Baud disabled
        baudrate *= 2;
        U1BRG=((FCY/baudrate)/16)-1;
    }


    U1STAbits.UTXISEL0 = 0; // Interrupt after one TX character is transmitted
    U1STAbits.UTXISEL1 = 0;
    //IEC0bits.U1TXIE = 1; // Enable UART TX interrupt
    U1STAbits.URXISEL = 0;
    IEC0bits.U1RXIE = 1;
    U1MODEbits.UARTEN = 1; // Enable UART
    U1STAbits.UTXEN = 1; // Enable UART TX
    /* Wait at least 105 microseconds (1/9600) before sending first char */
    //DELAY_105uS
}

void deinitUART1(void)
{
    U1MODEbits.ABAUD = 0;
    U1MODEbits.UARTEN = 0;
}

void startAutoBaudrateUART1(void)
{
    U1MODEbits.ABAUD = 1; // Auto-Baud disabled
}

void PPS(void)
{
    OSCCONbits.IOLOCK = 0;
    //RPINR18bits.U1RXR = 0x39;                                   //CHANGED BY MUNSIF AND AQIB
    //RPOR6bits.RP56R=1;
    RPINR18bits.U1RXR = 0x26;
    RPOR1bits.RP37R=1;
    OSCCONbits.IOLOCK = 1;
}

void txByteUART1(char send)
{
    while(U1STAbits.UTXBF== 1);
    U1TXREG = send;
}

uint8_t txRegisterUART1Empty(void)
{
    return U1STAbits.TRMT;
}

char rxByteUART1(void)
{
    return (char)U1RXREG;
}

void __attribute__((interrupt)) U1RXInterrupt( void )               
{
    if ( U1STAbits.OERR == 1 )
    {
        U1STAbits.OERR = 0;
    }
    uint8_t data = (uint8_t)U1RXREG;
    g_FIFO_USART1.buffer[g_FIFO_USART1.widx++] = data;    
    if(g_FIFO_USART1.widx >= FIFO_BUFFER_SIZE)   
    {                                  
        g_FIFO_USART1.widx = 0;                   
    }                                   
    IFS0bits.U1RXIF = 0;  
}
