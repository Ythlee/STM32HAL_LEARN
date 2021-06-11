#include "exti.h"
#include "delay.h"
#include "key.h"
#include "led.h"

void EXTI_Init(void)
{
    GPIO_InitTypeDef KEY_InitStruct;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    KEY_InitStruct.Mode = GPIO_MODE_IT_FALLING;
    KEY_InitStruct.Pin = GPIO_PIN_0;
    KEY_InitStruct.Pull = GPIO_PULLDOWN;
    KEY_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &KEY_InitStruct);

    HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    switch_key();
    if(KEY0 == 1)
    {
        LED0 = !LED0;
    }
}
