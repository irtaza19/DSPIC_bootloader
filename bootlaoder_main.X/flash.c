/*
 * File:   Flash.c
 * Author: semir
 *
 * Created on June 16, 2022, 8:15 AM
 */


#include "xc.h"
#include "flash.h"
#include "debug.h"

#define FLASH_WORD_PROG_CODE    0x4001
#define FLASH_ROW_PROG_CODE     0x4002 //write row
#define FLASH_PAGE_ERASE_CODE   0x4003 //erase entire page


//#define BYTES_PER_INSTRUCTION  2 
//#define INSTR_PER_ROW 128
//#define ROW_PER_PAGE   8
//#define BYTES_PER_ROW (INSTR_PER_ROW * BYTES_PER_INSTRUCTION)
//#define PM_ERASE_SIZE (INSTR_PER_ROW * BYTES_PER_INSTRUCTION * ROW_PER_PAGE)    //default erase size
/* *********************************************************************************
********************************************************************************** */
void pageEraseFLASH(uint32_t addr) 
{
    // Note: The program memory must be erased at an even page address boundary (data sheet)
    // Set up the NVMADR registers to the starting address of the page (0x022000);
    asm("push    TBLPAG");
    NVMADRU = (addr >> 16) & 0xffff;
    NVMADR = addr & 0xffff;
    
    // Set up NVMCON to erase one page of Program Memory
    NVMCON  = FLASH_PAGE_ERASE_CODE;
    // Disable interrupts < priority 7 for next 5 instructions>
    asm  ("DISI #06");
    // Write the KEY Sequence
    __builtin_write_NVM();
    NVMCONbits.WR = 1; // start the erase cycle
    __builtin_nop();
    __builtin_nop(); 
    while(NVMCONbits.WR); // wait until erase cycle is complete
    asm("pop     TBLPAG");
}
void writeSignleRowFLASH(uint32_t addr, uint8_t * data,uint16_t size) 
{
    uint16_t varWordL1;
    uint16_t varWordH1;
    uint16_t varWordL2;
    uint16_t varWordH2;
    int i;
    uint16_t k = 0;
    uint16_t INTCON2Save;
    
   
    NVMCON = 0x4001;
    TBLPAG = 0xFA;
    k = 0;
    for( i = 0; i < size/8; i++)
    {

        NVMADR = addr & 0xffff;
        NVMADRU = (addr >> 16) & 0xffff;
       
        varWordL1 = 0x0000;
        varWordL1 = data[k++];
        varWordL1 |=data[k++] << 8;
        varWordH1 = 0x0000;
        varWordH1 = data[k++];
        varWordH1 |=data[k++] << 8;
        varWordL2 = 0x0000;
        varWordL2 = data[k++];
        varWordL2 |=data[k++] << 8;
        varWordH2 = 0x0000;
        varWordH2 = data[k++];
        varWordH2 |=data[k++] << 8;
        
        //printDEBUG(DSYS,"[%xh%xh - %xh%xh][%x]\n",varWordL1,varWordH1,varWordL2,varWordH2,addr);
        __builtin_tblwtl(0x00,varWordL1 & 0xffff);
        __builtin_tblwth(0x00,varWordH1 & 0xffff);
        __builtin_tblwtl(0x02,varWordL2);
        __builtin_tblwth(0x02,varWordH2);
        INTCON2Save = INTCON2;
        __builtin_disable_interrupts();
        __builtin_write_NVM();
        while(NVMCONbits.WR==1);
        INTCON2 = INTCON2Save;
        addr += 4; 
    }

}
uint32_t readWordFLASH(uint32_t addr)
{
    /*
     *     data1 = FM_MemRead(0xF8, 0x0006); // tested
     */
    uint32_t tmp;
    uint16_t page = addr >> 16;
    uint16_t offset = addr & 0xffff;
    TBLPAG = page;
    tmp = __builtin_tblrdh(offset);
    tmp = (tmp << 16 ) | __builtin_tblrdl(offset);
    return tmp;
}
void writeLatchesFLASH(char * data)
{
    asm ("mov W0, W2"); // w2 points to the address of data to be written
    asm("push    TBLPAG");
    TBLPAG = 0xFA; // address of latch is 0xFA
    asm("mov #0,W1"); // Lower 16-bit of Write Latches starts from 0
    asm("MOV #128,W3");
    asm("loop:");  
    asm("TBLWTH.b [W2++], [W1++]");  // TBLWTH Ws,Wd -> Write Ws 7:0 to Prog 23:16
    asm("TBLWTL.b [W2++], [W1--]"); //y TBLWTH Ws,Wd -> Write Ws to Prog 15:0
    asm("TBLWTL.b [W2++], [W1]");
    asm("INC2 W1, W1");
    asm("DEC W3, W3");
    asm("BRA NZ, loop");
    asm("pop     TBLPAG");
}
