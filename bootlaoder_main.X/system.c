/* 
 * File:   uart.c
 * Author: semir
 *
 * Created on June 16, 2022, 8:15 AM
 */

#include "system.h"
#include "debug.h"
#include "delay.h"
#include "flash.h"

//_FOSCSEL(FNOSC_FRC & IESO_OFF);
//_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_NONE);

/*
_FGS(GWRP_OFF & GCP_OFF); // Disable Code Protection
_FOSCSEL(FNOSC_FRC);	
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_XT & OSCIOFNC_OFF & IOL1WAY_OFF);
_FWDT(FWDTEN_OFF);
_FICD(ICS_PGD1 & JTAGEN_OFF);// & RSTPRI_AF);
*/

// FICD
#pragma config ICS = PGD1               // ICD Communication Channel Select bits (Communicate on PGEC1 and PGED1)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)

// FPOR
#pragma config ALTI2C1 = OFF            // Alternate I2C1 pins (I2C1 mapped to SDA1/SCL1 pins)
#pragma config ALTI2C2 = OFF            // Alternate I2C2 pins (I2C2 mapped to SDA2/SCL2 pins)
#pragma config WDTWIN = WIN25           // Watchdog Window Select bits (WDT Window is 25% of WDT period)

// FWDT
#pragma config WDTPOST = PS32768        // Watchdog Timer Postscaler bits (1:32,768)
#pragma config WDTPRE = PR128           // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = ON              // PLL Lock Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)

// FOSC
#pragma config POSCMD = XT              // Primary Oscillator Mode Select bits (XT Crystal Oscillator Mode)
#pragma config OSCIOFNC = OFF           // OSC2 Pin Function bit (OSC2 is clock output)
#pragma config IOL1WAY = OFF            // Peripheral pin select configuration (Allow multiple reconfigurations)
#pragma config FCKSM = CSECMD           // Clock Switching Mode bits (Clock switching is enabled,Fail-safe Clock Monitor is disabled)

// FOSCSEL
#pragma config FNOSC = PRIPLL           // Oscillator Source Selection (Primary Oscillator with PLL module (XT + PLL, HS + PLL, EC + PLL))
#pragma config IESO = OFF               // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)

// FGS
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GCP = OFF                // General Segment Code-Protect bit (General Segment Code protect is Disabled)


								
volatile uint8_t c_fw_id[20];  
volatile uint32_t c_fw_version;
volatile uint32_t c_fw_size;
volatile uint32_t c_fw_crc;


void initSYSTEM(void)
{
    initSYSCLOCK();
    initDEBUG("BOOT",'5', 921600,"eClever bootloader ");
    
    uint32_t tmp;
    uint8_t k = 0;
    uint32_t addr = 0x5200;
    for( k = 0; k < 8; k++)
    {
      printDEBUG(DSYS,"Read from flash ...");
      tmp = readWordFLASH(addr);
      addr += 2;
      printDEBUG(DAPPEND,"[%x]\n",tmp);
      if(k < 5)
      {
        c_fw_id[(k * 3)+2] = (tmp >> 16) & 0xff;
        c_fw_id[(k * 3)+1] = (tmp >> 8) & 0xff;
        c_fw_id[(k * 3)] = tmp & 0xff;
      }
      else if(k==5)
      {
        c_fw_version = tmp;
      }
      else if(k==6)
      {
         c_fw_size = tmp;
      }
      else if(k == 7)
      {
          c_fw_crc = tmp;
      }     
    }
    printDEBUG(DSYS,"Firmware ID[%s]\n",c_fw_id);
    printDEBUG(DSYS,"Firmware CRC[%x]\n",c_fw_crc);
    printDEBUG(DSYS,"Firmware size[%d]\n",c_fw_size);
    printDEBUG(DSYS,"Firmware version[%x]\n",c_fw_version);
    
    initSYSTIM();
}

void initSYSCLOCK(void)
{
  
    PLLFBD = 60 - 2; // M = 43
    //PLLFBD = 40 - 2; // M = 43
    CLKDIVbits.PLLPOST = 0; // N2 = 2
    CLKDIVbits.PLLPRE = 4-2; // N1 = 8
    //PLLFBD = 63; // M = 43
    //CLKDIVbits.PLLPOST = 0; // N2 = 2
    //CLKDIVbits.PLLPRE = 1; // N1 = 2

    // Initiate Clock Switch to Internal FRC with PLL (NOSC = 0b001)
    //__builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONH(0x03);
    __builtin_write_OSCCONL(OSCCON | 0x01);
    
    // Wait for Clock switch to occur
    while (OSCCONbits.COSC != 0b011)
    {
        

    }
    PORTB = (0x01 << 14); //RE0 Logic HIGH
    
    PORTB = 0x00; //RE0 Logic HIGH
    // Wait for PLL to lock
    while (OSCCONbits.LOCK != 1) 
    {

    };

}