#ifndef __SYSTEM_H
#define	__SYSTEM_H

#include <xc.h>   
#define _XTAL_FREQ 40000000
//#define FCY 140000000UL
#define FCY 120000000UL
//#define FCY 80000000UL

extern volatile uint8_t c_fw_id[20];  
extern volatile uint32_t c_fw_version;
extern volatile uint32_t c_fw_size;
extern volatile uint32_t c_fw_crc;

void initSYSTEM(void);
void initSYSCLOCK(void);

#endif