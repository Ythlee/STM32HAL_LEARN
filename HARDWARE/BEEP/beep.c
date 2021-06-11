#include "beep.h"

void BEEP_Init(void)
{
    GPIO_InitTypeDef BEEP_InitStruct;

    __HAL_RCC_GPIOB_CLK_ENABLE();

    BEEP_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    BEEP_InitStruct.Pin = GPIO_PIN_8;
    BEEP_InitStruct.Pull = GPIO_PULLDOWN;
    BEEP_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &BEEP_InitStruct);

    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
}
