#ifndef __SDCARD_H_
#define __SDCARD_H_

#include "sys.h"

#define SD_TIMEOUT ((uint32_t)100000000)	//超时时间
#define SD_TRANSFER_OK ((uint8_t)0x00)
#define SD_TRANSFER_BUSY ((uint8_t)0x01)

u8 SD_Card_Init(void);
void show_sdcard_info(void);

#endif

