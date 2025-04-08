/* 
 * File:   main.c
 * Author: semir
 *
 * Created on June 10, 2022, 1:43 PM
 * Command line tool for flashing:
 * ipecmd -P33EP256GP504 -TPPK3 -M -F/home/semir/Dropbox/MCU/microchip/dsPIC33/projects/eclevar/firmware/bootloader-firmware/dist/default/production/eClever.X.production.hex
 */



#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "system.h"
#include "uart.h"
#include "delay.h"
#include "debug.h"
#include "fifo.h"
#include "boot.h"

/*
 * 
 */
#define LED_DIR TRISCbits.TRISC1  // Direction register bit for RC0
#define LED PORTCbits.RC1         // LED control on RC0

//int main ( void )
//{
    
  //  initSYSTEM();
    
  //  initUART1(0);
   // initBOOT();
    
       /* Infinite Loop */
  //  uint32_t time = getSYSTIM();

    

    
  //  while(1)
  //  {
  //      chkBOOT();      
   //     if(chk4TimeoutSYSTIM(time,1000) == SYSTIM_TIMEOUT)
   //     {
   //         printDEBUG(DSYS,"inside main while 1 \n");
    //        time = getSYSTIM();
           // PORTB ^= (0x01 << 14);
    //        PORTC ^= (0x01 << 0);   // toggle RC0
      //  }
    //} 
//}

int main(void)
{
    initSYSTEM();
    //initUART1(0);
    initUART1(9600);
    initBOOT();
    
    LED_DIR = 0;   // Set RC0 as output
    LED = 0;       // Turn off LED initially

    uint32_t time = getSYSTIM();

    while(1)
    {
        chkBOOT();

        if (chk4TimeoutSYSTIM(time, 1000) == SYSTIM_TIMEOUT)
        {
            printDEBUG(DSYS, "inside main while 1 \n");
            time = getSYSTIM();

            // Toggle LED on RC0 using your macro
            LED ^= 1;
        }
    } 
}