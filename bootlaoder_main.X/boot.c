/* 
 * File:   boot.c
 * Author: semir
 *
 * Created on June 16, 2022, 8:15 AM
 */

#include "mcuid.h"
#include "system.h"
#include "debug.h"
#include "delay.h"
#include "boot.h"
#include "fifo.h"
#include "uart.h"

// 0x05 (DLE), 0x55 (STX), 0x04 (ETX) control characters must be escaped
#define CHECK_BOOT_CONTROL_CHAR_ESCAPE(byte) \
    ((byte == 0x05) || (byte == 0x55) || (byte == 0x04))

volatile BOOT_INFO g_BOOT_INFO;

const uint8_t c_boot_version[2] = {0x01,0x06};
const uint8_t c_boot_device_id[2] = {0x21,0x20};
const uint8_t c_boot_ic_id[2] = {0x02,0x00};
const uint8_t c_boot_hwd_revision[2] = {0x01,0x00};
const uint8_t c_boot_serial_number[8] = {'1','0','1','5','4','3','0','3'};
const uint8_t c_boot_device_name[32] = {'S','T','N','2','1','2','0',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const uint8_t c_boot_fw_info[9] = {0x05,0x06,0x05,0x00,0x14,0x0a,0x0e,0x00,0x00};
const uint8_t c_boot_osc_info[20] = {0x00,0x24,0xf4,0x00,0x00,0x90,0xd0,0x03,0x81,0x06,0x1e,0x00,0x60,0x33,0x02,0x30,0x3e,0x00,0x00,0x00};
const uint8_t c_boot_date_time[6] = {0x11,0x06,0x1a,0x0d,0x19,0x2a};
const uint8_t c_flash[1024] = {0,0,0};
const uint8_t c_fw_unique[20] = {"IRQDesign"};

unsigned int mainApp __attribute__((address(0x1FFE)));

void initBOOT(void)
{
    mainApp = 0x0000;
    g_BOOT_INFO.state = BOOT_STATE_IDLE;
    g_BOOT_INFO.powerOnTimer = getSYSTIM();
    g_BOOT_INFO.fw_status = chkFwIntegrityBOOT();
    if(g_BOOT_INFO.fw_status != 0x00)
    {
        printDEBUG(DSYS,"Firmware ID:%s\n",c_fw_id);
        printDEBUG(DSYS,"Firmware version:%x\n",c_fw_version);
        printDEBUG(DSYS,"Firmware size: %d\n",c_fw_size);
        printDEBUG(DSYS,"Firmware crc: %x\n",c_fw_crc);
    }
    else
    {
        printDEBUG(DSYS,"Firmware is not valid\n");
    }    
}

void chkBOOT(void)
{
      // txByteUART1('A');                 //checking for the UART communication
    if(g_BOOT_INFO.status == BOOT_STATUS_DISCONNECTED)
    {
        if(chk4TimeoutSYSTIM(g_BOOT_INFO.powerOnTimer,BOOT_POWER_ON_TIMEOUT) == SYSTIM_TIMEOUT)
        {
            printDEBUG(DSYS,"Jump to the main application\n");
            startMainAppBOOT();
        } 
    }
    
   switch(g_BOOT_INFO.state)
   {
       case(BOOT_STATE_IDLE):
       {
            if(dataAvailableFIFO((FIFO *) &g_FIFO_USART1))
            {
                printDEBUG(DSYS,"uart datain \n");
                g_BOOT_INFO.state = BOOT_STATE_WAIT4STX;
                g_BOOT_INFO.timer = getSYSTIM();
            }
            break;
       }
       case(BOOT_STATE_WAIT4STX):
       {
           printDEBUG(DSYS,"inside wait for stx \n");
           if(chk4TimeoutSYSTIM(g_BOOT_INFO.timer,BOOT_COM_TIMEOUT) == SYSTIM_TIMEOUT)
           {
               printDEBUG(DSYS,"Receive byte timeout[%dh]\n",g_BOOT_INFO.data_cnt);
               g_BOOT_INFO.state = BOOT_STATE_IDLE;
               printDEBUG(DSYS, "Current Boot State: %xh\n", g_BOOT_INFO.state);
               startAutoBaudrateUART1();
           }
           if(dataAvailableFIFO((FIFO *) &g_FIFO_USART1))
           {
                if(getByteFIFO((FIFO *) &g_FIFO_USART1) == BOOT_CONTROL_CHAR_STX)
                {
                    g_BOOT_INFO.state = BOOT_STATE_WAIT4ETX;
                    g_BOOT_INFO.data_cnt = 0;
                    //printDEBUG(DSYS,"Receive data[%xh]\n",g_BOOT_INFO.data_cnt);
                }
                 printDEBUG(DSYS, "Current Boot State: %xh\n", g_BOOT_INFO.state);
                g_BOOT_INFO.timer = getSYSTIM();
           }
            break;
       }
       case(BOOT_STATE_WAIT4ETX):
       {
           if(chk4TimeoutSYSTIM(g_BOOT_INFO.timer,BOOT_COM_TIMEOUT) == SYSTIM_TIMEOUT)
           {
               g_BOOT_INFO.state = BOOT_STATE_IDLE;
               startAutoBaudrateUART1();
           }
           if(dataAvailableFIFO((FIFO *) &g_FIFO_USART1))
           {
                uint8_t data = getByteFIFO(&g_FIFO_USART1);
                if((data == BOOT_CONTROL_CHAR_DLE) && (g_BOOT_INFO.escapeFlag == 0))
                {
                    g_BOOT_INFO.escapeFlag = 0x01;
                }
                else if((data == BOOT_CONTROL_CHAR_ETX) && (g_BOOT_INFO.escapeFlag == 0))
                {
                    g_BOOT_INFO.state = BOOT_STATE_CMD_INIT;
                }
                else
                {
                    //printDEBUG(DSYS,"[%xh][%xh] - [%xb]@[%x]\n",dataAvailableFIFO( &g_FIFO_USART1),g_BOOT_INFO.data_cnt,data,getSYSTIM());
                    g_BOOT_INFO.data[g_BOOT_INFO.data_cnt++] = data;
                    g_BOOT_INFO.escapeFlag = 0x00;   
                }
                g_BOOT_INFO.timer = getSYSTIM();
           }
           break;
       }
       case(BOOT_STATE_CMD_INIT):
       {
           printDEBUG(DSYS, "Current Boot State: %xh\n", g_BOOT_INFO.state);
            printDEBUG(DSYS, "Received Packet: ");
            for (int i = 0; i < g_BOOT_INFO.data_cnt; i++) {
                printDEBUG(DSYS, "%02x ", g_BOOT_INFO.data[i]);
            }
            printDEBUG(DSYS, "\n");
           //printDEBUG(DSYS,"Data[");
           //uint16_t k;
           //for( k = 0; k < g_BOOT_INFO.data_cnt; k++)
          // {
            //  printDEBUG(DAPPEND,"%xb",g_BOOT_INFO.data[k]);
           //}
          // printDEBUG(DAPPEND,"]\n");
           g_BOOT_INFO.cmd = g_BOOT_INFO.data[0];
           g_BOOT_INFO.size = g_BOOT_INFO.data[1];
           g_BOOT_INFO.size = (g_BOOT_INFO.size << 8) | g_BOOT_INFO.data[2];
           g_BOOT_INFO.crc = g_BOOT_INFO.data[g_BOOT_INFO.size+3];
           g_BOOT_INFO.crc = (g_BOOT_INFO.crc << 8) | g_BOOT_INFO.data[g_BOOT_INFO.size+4];
           uint16_t crc = calCrcBOOT(g_BOOT_INFO.data, g_BOOT_INFO.data_cnt - 2);
           printDEBUG(DSYS,"Calculated CRC[%xh]\n",crc);
           printDEBUG(DSYS,"Received CRC[%xh]\n",g_BOOT_INFO.crc);
           if(crc == g_BOOT_INFO.crc)
           {
               //printDEBUG(DSYS,"CRC match [%xh] - [%xh]\n",crc,g_BOOT_INFO.crc);
               g_BOOT_INFO.state = BOOT_STATE_CMD_EXE;
           }
           else
           {
                //printDEBUG(DSYS,"Wrong crc[%xh] - [%xh]\n",crc,g_BOOT_INFO.crc);
                g_BOOT_INFO.state = BOOT_STATE_IDLE;
                printDEBUG(DSYS, "Current Boot State: %xh\n", g_BOOT_INFO.state);
                startAutoBaudrateUART1();
           }
           break;
       }
       case(BOOT_STATE_CMD_EXE):
       {
           if(g_BOOT_INFO.status == BOOT_STATUS_DISCONNECTED)
           {
                if(g_BOOT_INFO.cmd == BOOT_CMD_CONNECT)
                {
                    g_BOOT_INFO.status = BOOT_STATUS_CONNECTED;
                    txAckBOOT(BOOT_CMD_CONNECT,0,0);
                }
                g_BOOT_INFO.state = BOOT_STATE_IDLE;
                startAutoBaudrateUART1();
                return;
           }
           switch(g_BOOT_INFO.cmd)
           {
               case(BOOT_CMD_RESET):
               {
                   txAckBOOT(BOOT_CMD_RESET,0,0);
                   // Wait until ack sent (tx buffer is empty)
                   while(txRegisterUART1Empty() == 0);
                   asm("reset");
                   break;
               }
               case(BOOT_CMD_CONNECT):
               {
                    txAckBOOT(BOOT_CMD_CONNECT,0,0);
                    g_BOOT_INFO.state = BOOT_STATE_IDLE;
                    startAutoBaudrateUART1();
                   break;  
               }
               case(BOOT_CMD_GET_VERSION):
               {
                   uint8_t data[2];
                   uint8_t k = 0;
                   for( k = 0; k < 2; k++)
                   {
                       data[k] = c_boot_version[k];
                   }
                   txAckBOOT(g_BOOT_INFO.cmd,data,k);
                   g_BOOT_INFO.state = BOOT_STATE_IDLE;
                   startAutoBaudrateUART1();
                   break;
               }
               case(BOOT_CMD_GET_DEV_ID):
               {                
                   uint8_t data[2];
                   uint8_t k = 0;
                   for( k = 0; k < 2; k++)
                   {
                       data[k] = c_boot_device_id[k];
                   }
                   txAckBOOT(g_BOOT_INFO.cmd,data,k);
                   g_BOOT_INFO.state = BOOT_STATE_IDLE;
                   startAutoBaudrateUART1();
                   break;
               }
               case(BOOT_CMD_GET_HW_REV):
               {
                   uint8_t data[2];
                   uint8_t k = 0;
                   for( k = 0; k < 2; k++)
                   {
                       data[k] = c_boot_hwd_revision[k];
                   }
                   txAckBOOT(g_BOOT_INFO.cmd,data,k);
                   g_BOOT_INFO.state = BOOT_STATE_IDLE;
                   startAutoBaudrateUART1();
                   break;
               }
               case(BOOT_CMD_GET_SERIAL_NUMBER):
               {
                   uint8_t data[8];
                   uint8_t k = 0;
                   for( k = 0; k < 8; k++)
                   {
                       data[k] = c_boot_serial_number[k];
                   }
                   txAckBOOT(g_BOOT_INFO.cmd,data,k);
                   g_BOOT_INFO.state = BOOT_STATE_IDLE;
                   startAutoBaudrateUART1();
                   break;                   
               }
               case(BOOT_CMD_GET_EXTENDED_NAME):
               case(BOOT_CMD_GET_DEVICE_NAME):
               {
                   uint8_t data[32];
                   uint8_t k = 0;
                   for( k = 0; k < 32; k++)
                   {
                       data[k] = c_boot_device_name[k];
                   }
                   txAckBOOT(g_BOOT_INFO.cmd,data,k);
                   g_BOOT_INFO.state = BOOT_STATE_IDLE;
                   startAutoBaudrateUART1();
                   break;
               }
               case(BOOT_CMD_GET_IC_ID):
               {
                   uint8_t data[2];
                   uint8_t k = 0;
                   for( k = 0; k < 2; k++)
                   {
                       data[k] = c_boot_ic_id[k];
                   }
                   txAckBOOT(g_BOOT_INFO.cmd,data,k);
                   g_BOOT_INFO.state = BOOT_STATE_IDLE;
                   startAutoBaudrateUART1();
                   break;
               }
               case(BOOT_CMD_DEV_INITIALIZED):
               {
                   uint8_t data[1];
                   uint8_t k = 0;
                   data[k++] = 1;
                   txAckBOOT(g_BOOT_INFO.cmd,data,k);
                   g_BOOT_INFO.state = BOOT_STATE_IDLE;
                   startAutoBaudrateUART1();
                   break;
               }
               case(BOOT_CMD_GET_FW_STATUS):
               {
                   uint8_t data[1];
                   uint8_t k = 0;
                   data[k++] = 1;
                   txAckBOOT(g_BOOT_INFO.cmd,data,k);
                   g_BOOT_INFO.state = BOOT_STATE_IDLE;
                   startAutoBaudrateUART1();
                   break;
               }
               case(BOOT_CMD_START_UPLOAD):
               {
                   uint8_t data[2];
                   // Chunk size is calculated based on the BOOT_COM_SIZE, but we need to reduce it for the meta data.
                   // We will reduce by 16 just to be on the safe side
                   g_BOOT_INFO.image_size = g_BOOT_INFO.data[3];
                   g_BOOT_INFO.image_size = (g_BOOT_INFO.image_size << 8) | g_BOOT_INFO.data[4];
                   g_BOOT_INFO.image_size = (g_BOOT_INFO.image_size << 8) | g_BOOT_INFO.data[5];
                   g_BOOT_INFO.baddr = 0x00005000;
                   g_BOOT_INFO.eaddr = g_BOOT_INFO.baddr + g_BOOT_INFO.image_size/2;
                   g_BOOT_INFO.chunk_id = 0x0000;
                   g_BOOT_INFO.crow = 160;
                   g_BOOT_INFO.erow = g_BOOT_INFO.crow + g_BOOT_INFO.image_size / FLASH_ROW_SIZE;
                   g_BOOT_INFO.erow += (g_BOOT_INFO.image_size % FLASH_ROW_SIZE) ? 1 : 0;
                   uint16_t size = 256;
                   uint8_t k = 0;
                   //printDEBUG(DSYS,"Image size[%d]\n",g_BOOT_INFO.image_size);
                   uint32_t caddr = g_BOOT_INFO.baddr;
                   while(caddr <= g_BOOT_INFO.eaddr)
                   {
                       //printDEBUG(DSYS,"Erase page at @[%x] ...",caddr);
                       pageEraseFLASH(caddr);
                       //printDEBUG(DAPPEND,"DONE \n");
                       caddr += 2048;
                   }
                   data[k++] = ( size >> 8) & 0xff;
                   data[k++] = size & 0xff;
                   txAckBOOT(g_BOOT_INFO.cmd,data,k);
                   g_BOOT_INFO.state = BOOT_STATE_IDLE;
                   printDEBUG(DSYS,"Start firmware upgrade [%x][%dh]\n",g_BOOT_INFO.image_size,size);
                   startAutoBaudrateUART1();
                   break;
               }
               case(BOOT_CMD_SEND_CHUNK):
               {
                   
                   // Chunk size is calculated based on the BOOT_COM_SIZE, but we need to reduce it for the meta data.
                   // We will reduce by 16 just to be on the safe side

                   //printDEBUG(DSYS,"Received package[%dh][%dh] write @[%x/%x]\n",g_BOOT_INFO.chunk_id,g_BOOT_INFO.data_cnt,g_BOOT_INFO.baddr,g_BOOT_INFO.eaddr);
                   uint16_t k = 0;

                   uint32_t chunkSize = g_BOOT_INFO.data[1];
                   chunkSize = (chunkSize << 8) | g_BOOT_INFO.data[2];
                   chunkSize = chunkSize-2; // we don't need chunk ID info
                   g_BOOT_INFO.chunk_id  = g_BOOT_INFO.data[3];
                   g_BOOT_INFO.chunk_id = (g_BOOT_INFO.chunk_id << 8) | g_BOOT_INFO.data[4];
                   //printDEBUG(DSYS,"Chunk [%d] size[%dw]\n",g_BOOT_INFO.chunk_id,chunkSize);
                   uint16_t residue = chunkSize % 16;
                   if(residue != 0x00)
                   {
                       chunkSize += 16 - residue;
                   }
                   writeSignleRowFLASH(g_BOOT_INFO.baddr, &g_BOOT_INFO.data[5],chunkSize);
                   //g_BOOT_INFO.baddr += 32 * 4; 
                   g_BOOT_INFO.baddr += chunkSize/2;
                   uint8_t data[2];
                   data[k++] = (g_BOOT_INFO.chunk_id >> 8) & 0xff;
                   data[k++] = g_BOOT_INFO.chunk_id & 0xff;
                   txAckBOOT(g_BOOT_INFO.cmd,data,k);
                   //g_BOOT_INFO.chunk_id++;
                   g_BOOT_INFO.state = BOOT_STATE_IDLE;
                   startAutoBaudrateUART1();
                   break;

               }
               case(BOOT_CMD_FW_INFO):
               {
                   uint8_t data[9];
                   uint8_t k = 0;
                   for( k = 0; k < 9; k++)
                   {
                       data[k] = c_boot_fw_info[k];
                   }
                   txAckBOOT(g_BOOT_INFO.cmd,data,k);
                   g_BOOT_INFO.state = BOOT_STATE_IDLE;
                   startAutoBaudrateUART1();
                   break;
               }
               case(BOOT_CMD_GET_OSC_INFO):
               {
                   uint8_t data[20];
                   uint8_t k = 0;
                   for( k = 0; k < 20; k++)
                   {
                       data[k] = c_boot_osc_info[k];
                   }
                   txAckBOOT(g_BOOT_INFO.cmd,data,k);
                   g_BOOT_INFO.state = BOOT_STATE_IDLE;
                   startAutoBaudrateUART1();
                   break;
               }
               case(BOOT_CMD_GET_MIN_FW_VERSION):
               {
                   txAckBOOT(g_BOOT_INFO.cmd,0,0);
                   g_BOOT_INFO.state = BOOT_STATE_IDLE;
                   startAutoBaudrateUART1();
                   break;
               }
               case(BOOT_CMD_GET_DATE_TIME):
               {
                   uint8_t data[6];
                   uint8_t k = 0;
                   for( k = 0; k < 6; k++)
                   {
                       data[k] = c_boot_date_time[k];
                   }
                   txAckBOOT(g_BOOT_INFO.cmd,data,k);
                   g_BOOT_INFO.state = BOOT_STATE_IDLE;
                   startAutoBaudrateUART1();
                   break;
               }
            }
           
           break;
       }
       default:
       {
            break;
       }
   }
    
}


uint16_t calCrcBOOT(uint8_t * ptr, uint32_t count)
{
	uint16_t  crc = 0x0000u;
	uint8_t  k;

	while (count > 0u)
	{
		crc = crc ^ ((uint16_t)(*ptr) << 8);
		ptr++;
		for(k = 0; k < 8u; k++)
		{
			if ((crc & 0x8000u) != 0x00u)
			{
				crc = crc << 1u ^ 0x1021u;
			}
			else
			{
				crc = crc << 1u;
			}
		}

		count--;
	}
	return (crc);
}

void txAckBOOT(uint8_t cmd,uint8_t * data,uint8_t size)
{
    uint8_t pkt[128];
    uint8_t crcBuffer[128];
    uint8_t k = 0;
    uint8_t p = 0;
    pkt[k++] = 0x55;
    pkt[k++] = 0x55;
    pkt[k++] = cmd | 0x40;
    crcBuffer[p++] = cmd | 0x40;
    pkt[k++] = size;
    crcBuffer[p++] = size;
    uint8_t n = 0;
    for( n = 0; n < size; n++)
    {
        if(CHECK_BOOT_CONTROL_CHAR_ESCAPE(data[n]))
        {
           pkt[k++] = 0x05;
        }
        pkt[k++] = data[n];
        crcBuffer[p++] = data[n];
    }
    uint16_t crc = calCrcBOOT(crcBuffer,p);
    uint8_t crcLSB = (crc & 0xff);
    uint8_t crcMSB = (crc >> 8) & 0xff;
    
    if(CHECK_BOOT_CONTROL_CHAR_ESCAPE(crcMSB))
    {
       pkt[k++] = 0x05;
    }
    pkt[k++] = crcMSB;

    if(CHECK_BOOT_CONTROL_CHAR_ESCAPE(crcLSB))
    {
       pkt[k++] = 0x05;
    }
    pkt[k++] = crcLSB;
    
    pkt[k++] = 0x04;
    for(n = 0; n < k; n++)
    {
        txByteUART1(pkt[n]);
    }
}

#define USER_APP_RESET_ADDRESS                  0x5000

void startMainAppBOOT(void)
{	
    g_BOOT_INFO.fw_status = chkFwIntegrityBOOT();
    //g_BOOT_INFO.fw_status = 1;
    if( g_BOOT_INFO.fw_status == 0x00)
    {
        g_BOOT_INFO.status = BOOT_STATUS_CONNECTED;
        printDEBUG(DSYS,"Cant jump, no main application\n");
    }
    else
    {
        deinitUART1();
        mainApp = 0xffff;
        void (*fptr)(void);
        fptr = (void (*)(void))USER_APP_RESET_ADDRESS;       
        fptr(); 
     }


	
}	

uint8_t chkFwIntegrityBOOT(void)
{
    uint32_t k;
    for( k = 0; k < 20; k++)
    {
        if(c_fw_unique[k] == 0)
        {
            break;
        }
        if(c_fw_id[k] != c_fw_unique[k])
        {
            printDEBUG(DSYS,"ID problem\n");
            return 0;
            break;
        }
    }
    
    uint32_t crc = getFwCrcBOOT();
    printDEBUG(DSYS,"CRC R[%x][%x]\n",c_fw_crc,crc);
    if(c_fw_crc != crc)
    {
        return 0;
    }
    return 1;
}

uint32_t getFwCrcBOOT(void)
{
    uint32_t baddr = 0x5000;
    uint32_t eaddr = baddr + c_fw_size/2;
    uint8_t crcByte[4] = {0,0,0,0};
    uint32_t tmp;
    uint32_t crc;
    uint32_t k = 0;
    while(k < c_fw_size)
    {
        if((k < 0x400) || (k >= 0x600))
        {
            //printDEBUG(DSYS,"Read from flash ...");
            tmp = readWordFLASH(baddr);
            crcByte[2] += (tmp >> 16) & 0xff;
            crcByte[1] += (tmp >> 8) & 0xff;
            crcByte[0] += (tmp) & 0xff;
            crc = crcByte[3];
            crc = (crc << 8) | crcByte[2];
            crc = (crc << 8) | crcByte[1];
            crc = (crc << 8) | crcByte[0];
            /*
            if((k%16) == 0)
            {
                printDEBUG(DSYS,"[%dw] %xw %xw\n",k,crc,tmp);
            }*/
        }  
        k+=4;
        baddr += 2;
    }
    /*
    printDEBUG(DSYS,"[%dw] %xw %xw\n",k,crc,tmp);
    printDEBUG(DSYS,"B[%xw] E[%xw] Size[%dw]\n",c_fw_size/2,eaddr,c_fw_size);
    */
    //crc = crcByte[3];
    // crc = (crc << 8) | crcByte[2];
    //crc = (crc << 8) | crcByte[1];
    //crc = (crc << 8) | crcByte[0];
    return crc;    
}
