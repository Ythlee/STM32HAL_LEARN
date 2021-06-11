#include "led.h"

void LED_Init(void)
{
    GPIO_InitTypeDef LED_InitStruct;
    __HAL_RCC_GPIOB_CLK_ENABLE(); //开启GPIOB时钟
    __HAL_RCC_GPIOE_CLK_ENABLE(); //开启GOIOE时钟

    LED_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    LED_InitStruct.Pin = GPIO_PIN_5;
    LED_InitStruct.Pull = GPIO_PULLUP;
    LED_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &LED_InitStruct);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);
}
