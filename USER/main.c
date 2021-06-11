#include "sys.h"
#include "main.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"
#include "dma.h"
#include "24cxx.h"
#include "w25qxx.h"
#include "lcd.h"
#include "adc.h"
#include "gpio.h"
#include "sdio.h"
#include "fatfs.h"
#include "dma.h"
#include "stdio.h"
#include "string.h"

char fileName[] = "study.txt";
uint32_t rwnum;
char wbuff[512];
char rbuff[512];

int main(void)
{
    HAL_Init();
    Stm32_Clock_Init(RCC_PLL_MUL9);
    uart_init(115200);
    delay_init(72);
    LED_Init();
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_SDIO_SD_Init();
	MX_FATFS_Init();
	
	retSD = f_mount(&SDFatFS,SDPath,1);
	switch(retSD)
	{
		case FR_NO_FILESYSTEM:
			retSD = f_mkfs(SDPath,0,512);
			if(retSD != FR_OK)
				while(1);
			break;
		case FR_OK:
			break;
		default:
			while(1);
	}
	retSD = f_open(&SDFile,fileName,FA_CREATE_ALWAYS|FA_WRITE);
	if(retSD != FR_OK)
		while(1);
	sprintf(wbuff,"%s","今天真是个好天气！");
	retSD = f_write(&SDFile,wbuff,strlen((const char *)wbuff),&rwnum);
	if(retSD != FR_OK)
		while(1);
	f_close(&SDFile);
	
	retSD = f_open(&SDFile,fileName,FA_READ);
	if(retSD != FR_OK)
		while(1);
	retSD = f_read(&SDFile,rbuff,sizeof(rbuff),&rwnum);
	if(retSD != FR_OK)
		while(1);
	
	f_close(&SDFile);
	f_mount(NULL,SDPath,1);
    while (1)
    {

    }
}
