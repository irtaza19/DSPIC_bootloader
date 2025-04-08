/* Define to prevent recursive inclusion *********************************** */
#ifndef __DEBUG_H__
#define __DEBUG_H__

/* Includes **************************************************************** */
#include <stdint.h>
#include <stdarg.h>
#include "mcuid.h"
/* Exported types ********************************************************** */
typedef enum 
{
	DSYS = 0x0000,

	DSIZE,
	DDEBUG = 0x0000,
	DAPPEND = 0x1000,
	DWARNING = 0x2000,
	DERROR = 0x4000,
	DNOTIFY = 0x8000,
	DHEADER = 0x8000,
} DEBUG_Modules_t;

/* Exported constants ****************************************************** */
#define LINUX_OS

#define DEVICE_ID_SIZE							12
#define DEVICE_ID_REG_ADDR						0x1FF1E800

#define SYSTEM_DEBUG
/* Exported macros ********************************************************* */
#define DEBUG_BAUDRATE_921600					0x00000068

#define DEBUG_WAIT4TC_COUNT						10000




/* Exported variables ****************************************************** */
extern volatile uint8_t g_debug_block_flag;
extern volatile uint8_t g_device_id[DEVICE_ID_SIZE];

/* Exported functions ****************************************************** */
void initDEBUG(char * str, uint8_t ccode, uint32_t baudrate, char * app);
void printDEBUG(uint16_t sid, char *str, ... );
void putcharDEBUG(char data);


#endif /* __DEBUG_H__ */

/* ***************************** END OF FILE ******************************* */
