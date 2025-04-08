#ifndef __BOOT_H
#define	__BOOT_H

#include <xc.h>
#include "flash.h"

#define BOOT_COM_TIMEOUT                            10
#define BOOT_COM_SIZE                               512
#define BOOT_POWER_ON_TIMEOUT                       20000

enum BOOT_CONTROL_CHAR
{
    BOOT_CONTROL_CHAR_STX = 0x55,
    BOOT_CONTROL_CHAR_ETX = 0x04,
    BOOT_CONTROL_CHAR_DLE = 0x05,
};
enum BOOT_STATE
{
    BOOT_STATE_IDLE = 0x00,
    BOOT_STATE_WAIT4STX,
    BOOT_STATE_WAIT4ETX,
    BOOT_STATE_CMD_INIT,
    BOOT_STATE_CMD_EXE, 
};

enum BOOT_STATUS
{
    BOOT_STATUS_DISCONNECTED = 0x00,
    BOOT_STATUS_CONNECTED,
};

enum BOOT_CMD
{
    BOOT_CMD_RESEND_LAST = 0x01,
    BOOT_CMD_RESET = 0x02,
    BOOT_CMD_CONNECT = 0x03,
    BOOT_CMD_GET_VERSION = 0x06,
    BOOT_CMD_GET_DEV_ID = 0x07,
    BOOT_CMD_GET_HW_REV = 0x08,
    BOOT_CMD_GET_DATE_TIME = 0x09,
    BOOT_CMD_GET_SERIAL_NUMBER = 0x0A,
    BOOT_CMD_GET_DEVICE_NAME = 0x0B,
    BOOT_CMD_GET_IC_ID = 0x0C,
    BOOT_CMD_GET_EXTENDED_NAME = 0x0D,
    BOOT_CMD_DEV_INITIALIZED = 0x0E,
    BOOT_CMD_GET_FW_STATUS = 0x0F,
    BOOT_CMD_START_UPLOAD = 0x30,
    BOOT_CMD_SEND_CHUNK = 0x31,
    BOOT_CMD_GET_OSC_INFO = 0x1B,
    BOOT_CMD_GET_MIN_FW_VERSION = 0x2E,
    BOOT_CMD_FW_INFO = 0x2F
};

typedef struct
{
    uint8_t state;
    uint8_t status;
    uint32_t timer;
    uint32_t powerOnTimer;
    uint8_t data[BOOT_COM_SIZE];
    uint16_t data_cnt;
    
    uint8_t cmd;
    uint16_t size;
    uint16_t crc;
    uint8_t escapeFlag;
    
    uint8_t image_data[FLASH_ROW_SIZE];
    uint32_t image_size;
    uint32_t erow;
    uint32_t crow;
    uint16_t chunk_id;
    uint32_t baddr;
    uint32_t eaddr;
    uint8_t fw_status;

} BOOT_INFO;

void initBOOT(void);
void chkBOOT(void);
uint16_t calCrcBOOT(uint8_t * ptr, uint32_t count);
void txAckBOOT(uint8_t cmd,uint8_t * data,uint8_t size);
void startMainAppBOOT(void);
uint8_t chkFwIntegrityBOOT(void);
uint32_t getFwCrcBOOT(void);

extern volatile BOOT_INFO g_BOOT_INFO;
#endif