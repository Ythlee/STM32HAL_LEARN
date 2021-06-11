#include "key.h"
#include "delay.h"
#include "usart.h"

void KEY_Init(void)
{
    GPIO_InitTypeDef KEY_InitStruct;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    KEY_InitStruct.Mode = GPIO_MODE_INPUT;
    KEY_InitStruct.Pin = GPIO_PIN_0;
    KEY_InitStruct.Pull = GPIO_PULLDOWN;
    KEY_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &KEY_InitStruct);

    KEY_InitStruct.Mode = GPIO_MODE_INPUT;
    KEY_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4;
    KEY_InitStruct.Pull = GPIO_PULLUP;
    KEY_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOE, &KEY_InitStruct);
}

u8 switch_key(void)
{
    static u8 key_up = 1;
    if (key_up && (KEY0 == 1 || KEY1 == 0 || KEY2 == 0 || KEY3 == 0))
    {
        delay_ms(20);
        key_up = 0;
        if (KEY0 == 1)
            return KEY0_PRESS_FLAG;
        else if (KEY1 == 0)
            return KEY1_PRESS_FLAG;
        else if (KEY2 == 0)
            return KEY2_PRESS_FLAG;
        else if (KEY3 == 0)
            return KEY3_PRESS_FLAG;
    }
    else if (KEY0 == 0 && KEY1 == 1 && KEY2 == 1 && KEY3 == 1)
        key_up = 1;
    return 0;
}

u8 KEY_Scan(u8 mode)
{
    static u8 key_up = 1; //按键松开标志
    if (mode == 1)
        key_up = 1; //支持连按
    if (key_up && (KEY0 == 1 || KEY1 == 0 || KEY2 == 0 || KEY3 == 0))
    {
        delay_ms(10);
        key_up = 0;
        if (KEY0 == 1)
            return KEY0_PRESS_FLAG;
        else if (KEY1 == 0)
            return KEY1_PRESS_FLAG;
        else if (KEY2 == 0)
            return KEY2_PRESS_FLAG;
        else if (KEY3 == 0)
            return KEY3_PRESS_FLAG;
    }
    else if (KEY0 == 0 && KEY1 == 1 && KEY2 == 1 && KEY3 == 1)
        key_up = 1;
    return 0; //无按键按下
}

void test_key(u8 key)
{
    switch (key)
    {
    case KEY0_PRESS_FLAG:
        printf("KEY0_PRESS_FLAG\r\n");
        break;
    case KEY1_PRESS_FLAG:
        printf("KEY1_PRESS_FLAG\r\n");
        break;
    case KEY2_PRESS_FLAG:
        printf("KEY2_PRESS_FLAG\r\n");
        break;
    case KEY3_PRESS_FLAG:
        printf("KEY3_PRESS_FLAG\r\n");
        break;
    default:
        break;
    }
}
