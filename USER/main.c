#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"
#include "key.h"
#include "exti.h"
#include "dma.h"
#include "24cxx.h"
#include "w25qxx.h"
#include "fatfs_test.h"
#include "lcd.h"
#include "adc.h"
#include "sdcard.h"

int main(void)
{
    HAL_Init();
    Stm32_Clock_Init(RCC_PLL_MUL9);
    uart_init(115200);
    delay_init(72);
    LED_Init();
    ADC_Init();
	
	


    while (SD_Card_Init() == 0)
    {
        //系统正常运行
        show_sdcard_info();
        LED0 = !LED0;
        delay_ms(500);
    }
}
