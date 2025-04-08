/**
 * @file    DEBUG.c 
 * @author  IRQdesign d.o.o 
 * @date    Augustu 2021 
 * @brief   This file contains  
 *
 * @attention
 *		- CLK source PCLK1 (64MHz)
 *		- PA10 (Rx swaped to Tx)
 *
 * @ingroup grpDEBUG
 */

/* Includes **************************************************************** */
#include "debug.h"
#include "misc.h"
#include "fwver.h"
#include "system.h"


/* Private types *********************************************************** */

/* Private constants ******************************************************* */

/* Private macros ********************************************************** */
#define initUART								initUART2
#define putcharUART 							putcharUART2
#define sprintUART 								sprintUART2

/* Private variables ******************************************************* */
volatile uint8_t g_core_id[17] = {0};
volatile uint8_t g_logmsg_ccode = '5';
const uint8_t c_mod[DSIZE][32] =
{
	{"SYS"},
};

volatile uint8_t g_debug_block_flag = 1;
volatile uint8_t g_device_id[DEVICE_ID_SIZE];


/* Private function prototypes ********************************************* */
static void initUART2(uint32_t baudrate);
static void putcharUART2(uint8_t data);
static void sprintUART2(uint8_t * str);

/* Exported functions ****************************************************** */

/** 
 * @brief Initialize debug module
 * @param str Name of the current core
 * @param ccode Debug text default color 
 * @param baudrate UART baudrate 
 * @param app Name of the application
 * @retval No return value 
 */
void initDEBUG(char * str, uint8_t ccode, uint32_t baudrate, char * app)
{
	uint8_t	k;
	g_logmsg_ccode = ccode;
	for(k=0;k<15;k++)
	{
		g_core_id[k] = str[k];
		if(str[k] == 0)
		{
			break;
		}
	}

	// get the device ID
	//uint8_t * ptr = (uint8_t *)(DEVICE_ID_REG_ADDR);
	//for(k=0; k<(DEVICE_ID_SIZE); k++)
	//{
	//	g_device_id[k] = ptr[k];
	//}
	initUART(baudrate);
	printDEBUG(DAPPEND,"\n\n");
	printDEBUG(DAPPEND | DHEADER,"wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww\n");
	printDEBUG(DAPPEND | DHEADER,"w IRQDESIGN d.o.o. Tuzla Bosnia and Herzegovina\n");
	printDEBUG(DAPPEND | DHEADER,"w----------------------------------------------------------------\n");
	printDEBUG(DAPPEND | DHEADER,"w %s\n",app);
	printDEBUG(DAPPEND | DHEADER,"w hwver: %d.%d\n",FIRMWARE_VERSION>>28,(FIRMWARE_VERSION>>24)&0x0000000F);
	printDEBUG(DAPPEND | DHEADER,"w fwver: %d.%d.%d.\n",(FIRMWARE_VERSION>>16)&0x000000FF,(FIRMWARE_VERSION>>8)&0x000000FF,(FIRMWARE_VERSION)&0x000000FF);
	printDEBUG(DAPPEND | DHEADER,"w time:  %x\n",BUILD_TIME);
	printDEBUG(DAPPEND | DHEADER,"w Device ID:  ");
	k = 0;
	for( k = 0; k < DEVICE_ID_SIZE; k++ )
	{
		printDEBUG(DAPPEND | DHEADER,"%xb",g_device_id[k]);
	}
	printDEBUG(DAPPEND | DHEADER,"\n");
	printDEBUG(DAPPEND | DHEADER ,"wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww\n");
}

/** @brief Print debug message
 *	@param  sid Debug module 
 *	@param  str String with desired formating 
 *	@param  ... VA list
 *	@return No return value 
 */
void printDEBUG(uint16_t sid, char *str, ... )
{
#ifndef SYSTEM_DEBUG
	return;
#endif

	uint8_t rstr[40];													// 33 max -> 32 ASCII for 32 bits and NULL
	uint16_t k = 0;
	uint16_t arg_type;
	uint32_t utmp32;
	uint32_t * p_uint32;
	char * p_char;
	va_list vl;

#ifdef LINUX_OS
	if((sid & DAPPEND) == 0x00)
	{
		putcharUART('\e');
		putcharUART('[');
		putcharUART('3');
		putcharUART(g_logmsg_ccode);
		putcharUART('m');

		for(k=0; k<16; k++)
		{
			putcharUART(g_core_id[k]);
		}


		putcharUART('\e');
		putcharUART('[');
		putcharUART('3');
		putcharUART('4');
		putcharUART('m');
		putcharUART('-');
		putcharUART('>');
		putcharUART(' ');
		uint16_t idx = sid & 0x0FFF;
		k = 0;
		while(c_mod[idx][k] != '\0')
		{
			putcharUART((uint8_t)c_mod[idx][k++]);
		}

		putcharUART(':');
		putcharUART(' ');
		putcharUART('\e');
		putcharUART('[');
		putcharUART('0');
		putcharUART('m');
	}
	if(sid & DERROR)
	{
		putcharUART('\e');
		putcharUART('[');
		putcharUART('3');
		putcharUART('1');
		putcharUART('m');
	}
	else if(sid & (DDEBUG))
	{
		putcharUART('\e');
		putcharUART('[');
		putcharUART('3');
		putcharUART('3');
		putcharUART('m');
	}
	else if(sid & (DWARNING))
	{
		putcharUART('\e');
		putcharUART('[');
		putcharUART('3');
		putcharUART('3');
		putcharUART('m');
	}
	else if(sid & (DNOTIFY))
	{
		putcharUART('\e');
		putcharUART('[');
		putcharUART('3');
		putcharUART('5');
		putcharUART('m');
	}


#endif	


	k = 0;
	//va_start(vl, 10);													// always pass the last named parameter to va_start, for compatibility with older compilers
	va_start(vl, str);													// always pass the last named parameter to va_start, for compatibility with older compilers
	while(str[k] != 0x00)
	{
		if(str[k] == '%')
		{
			if(str[k+1] != 0x00)
			{
				switch(str[k+1])
				{
					case('b'):
					{
						// binary
						if(str[k+2] == 'b')
						{
							// byte
							utmp32 = va_arg(vl, int);
							arg_type = (PRINT_ARG_TYPE_BINARY_BYTE);
						}
						else if(str[k+2] == 'h')
						{
							// half word
							utmp32 = va_arg(vl, int);
							arg_type = (PRINT_ARG_TYPE_BINARY_HALFWORD);
						}
						else if(str[k+2] == 'w')
						{
							// word
							utmp32 = va_arg(vl, uint32_t);
							arg_type = (PRINT_ARG_TYPE_BINARY_WORD);
						}
						else
						{
							// default word
							utmp32 = va_arg(vl, uint32_t);
							arg_type = (PRINT_ARG_TYPE_BINARY_WORD);
							k--;
						}

						k++;
						p_uint32 = &utmp32;
						break;
					}
					case('d'):
					{
						// decimal
						if(str[k+2] == 'b')
						{
							// byte
							utmp32 = va_arg(vl, int);
							arg_type = (PRINT_ARG_TYPE_DECIMAL_BYTE);
						}
						else if(str[k+2] == 'h')
						{
							// half word
							utmp32 = va_arg(vl, int);
							arg_type = (PRINT_ARG_TYPE_DECIMAL_HALFWORD);
						}
						else if(str[k+2] == 'w')
						{
							// word
							utmp32 = va_arg(vl, uint32_t);
							arg_type = (PRINT_ARG_TYPE_DECIMAL_WORD);
						}
						else
						{
							// default word
							utmp32 = va_arg(vl, int);
							arg_type = (PRINT_ARG_TYPE_DECIMAL_HALFWORD);
							k--;
						}

						k++;
						p_uint32 = &utmp32;
						break;
					}
					case('c'):
					{
						// character
						char tchar = va_arg(vl, int);
						putcharUART(tchar);
						arg_type = (PRINT_ARG_TYPE_CHARACTER);
						break;
					}
					case('s'):
					{
						// string
						p_char = va_arg(vl, char *);
						sprintUART((uint8_t *)p_char);
						arg_type = (PRINT_ARG_TYPE_STRING);
						break;
					}
					case('f'):
					{
						// float
						uint64_t utmp64 = va_arg(vl, uint64_t);			// convert double to float representation IEEE 754
						uint32_t tmp1 = utmp64&0x00000000FFFFFFFF;
						tmp1 = tmp1>>29;
						utmp32 = utmp64>>32;
						utmp32 &= 0x07FFFFFF;
						utmp32 = utmp32<<3;
						utmp32 |= tmp1;
						if(utmp64 & 0x8000000000000000)
							utmp32 |= 0x80000000;

						if(utmp64 & 0x4000000000000000)
							utmp32 |= 0x40000000;

						p_uint32 = &utmp32;

						arg_type = (PRINT_ARG_TYPE_FLOAT);
						//arg_type = (PRINT_ARG_TYPE_HEXADECIMAL_WORD);
						//arg_type = (PRINT_ARG_TYPE_BINARY_WORD);
						break;
					}
					case('x'):
					{
						// hexadecimal
						if(str[k+2] == 'b')
						{
							// byte
							utmp32 = (uint32_t)va_arg(vl, int);
							arg_type = (PRINT_ARG_TYPE_HEXADECIMAL_BYTE);
						}
						else if(str[k+2] == 'h')
						{
							// half word
							utmp32 = (uint32_t)va_arg(vl, int);
							arg_type = (PRINT_ARG_TYPE_HEXADECIMAL_HALFWORD);
						}
						else if(str[k+2] == 'w')
						{
							// word
							utmp32 = va_arg(vl, uint32_t);
							arg_type = (PRINT_ARG_TYPE_HEXADECIMAL_WORD);
						}
						else
						{
							utmp32 = va_arg(vl, int);
							arg_type = (PRINT_ARG_TYPE_HEXADECIMAL_HALFWORD);
							k--;
						}

						k++;
						p_uint32 = &utmp32;
						break;
					}
					default:
					{
						utmp32 = 0;
						p_uint32 = &utmp32;
						arg_type = (PRINT_ARG_TYPE_UNKNOWN);
						break;
					}
				}

				if(arg_type&(PRINT_ARG_TYPE_MASK_CHAR_STRING))
				{
					getStr4NumMISC(arg_type, p_uint32, rstr);
					sprintUART(rstr);
				}
				k++;
			}
		}
		else
		{
			// not a '%' char -> print the char
			putcharUART(str[k]);
			if (str[k] == '\n')
				putcharUART('\r');
		}
		k++;
	}

	va_end(vl);

#ifdef LINUX_OS
	if(sid & 0xC000)
	{
		putcharUART('\e');
		putcharUART('[');
		putcharUART('0');
		putcharUART('m');
	}
#endif	
}

/** 
 * @brief Send one character through debug interface
 * @param  data Character to be sent 
 * @retval No return value 
 */
void putcharDEBUG(char data)
{
	putcharUART(data);

}


/* Private functions ******************************************************* */
void initUART2(uint32_t baudrate)
{
     //RX,TX
    TRISBbits.TRISB9=0;
  
     //peripheral pin select
     
    // AD1PCFGL = 0xFFFF;
    AD1CON1bits.ADON = 0;
     //peripheral pin select
    //PPS();
    OSCCONbits.IOLOCK = 0;
    RPINR18bits.U1RXR = 0x25;
    RPOR3bits.RP41R=0x03;
    OSCCONbits.IOLOCK = 1;
 
    U2MODEbits.STSEL = 0; // 1-Stop bit
    U2MODEbits.PDSEL = 0; // No Parity, 8-Data bits
    U2MODEbits.ABAUD = 0; // Auto-Baud disabled
    U2MODEbits.BRGH = 0; // Standard-Speed mode
    //U1BRG = BRGVAL; // Baud Rate setting for 9600
    baudrate *= 2;
    U2BRG=((FCY/baudrate)/16)-1;
    U2STAbits.UTXISEL0 = 0; // Interrupt after one TX character is transmitted
    U2STAbits.UTXISEL1 = 0;
    //IEC0bits.U1TXIE = 1; // Enable UART TX interrupt
    U2STAbits.URXISEL = 0;
    //IEC0bits.U1RXIE = 1;
    U2MODEbits.UARTEN = 1; // Enable UART
    U2STAbits.UTXEN = 1; // Enable UART TX
    /* Wait at least 105 microseconds (1/9600) before sending first char */
    //DELAY_105uS
}

void putcharUART2(uint8_t data)
{/// print one character to UART4
    while(U2STAbits.UTXBF== 1);
    U2TXREG = data;												    
}

void sprintUART2(uint8_t * str)
{
	uint16_t k = 0;
	
	while (str[k] != '\0')
    {
        putcharUART2(str[k]);
        if (str[k] == '\n')
            putcharUART2('\r');
        k++;

        if (k == MAX_PRINT_STRING_SIZE)
            break;
    }
}



/* ***************************** END OF FILE ******************************* */


