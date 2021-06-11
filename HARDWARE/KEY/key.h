#ifndef __KEY_H_
#define __KEY_H_

#include "sys.h"

#define KEY0 HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)
#define KEY1 HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4)
#define KEY2 HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3)
#define KEY3 HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_2)

#define KEY0_PRESS_FLAG 1
#define KEY1_PRESS_FLAG 2
#define KEY2_PRESS_FLAG 3
#define KEY3_PRESS_FLAG 4

void KEY_Init(void);

u8 switch_key(void);

void test_key(u8 key);
u8 KEY_Scan(u8 mode);

#endif // !__KEY_H_
