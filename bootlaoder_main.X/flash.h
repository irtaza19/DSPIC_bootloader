#ifndef __FLASH_H
#define	__FLASH_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define FLASH_BLOCK_SIZE                1536
#define FLASH_BLOCK_ADDRESS_SIZE        1024
#define FLASH_ROW_SIZE                  192
#define FLASH_ROW_ADDRESS_SIZE          128


void pageEraseFLASH(uint32_t addr); 
void writeSignleRowFLASH(uint32_t addr, uint8_t * data,uint16_t size);
uint32_t readWordFLASH(uint32_t addr);
void writeLatchesFLASH(char *data);

#endif	/* XC_HEADER_TEMPLATE_H */

