#include "iic.h"
#include "delay.h"

void IIC_Init(void)
{
    GPIO_InitTypeDef IIC_InitStruct;
    __HAL_RCC_GPIOB_CLK_ENABLE();

    IIC_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
    IIC_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    IIC_InitStruct.Pull = GPIO_PULLUP;
    IIC_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(GPIOB, &IIC_InitStruct);
    IIC_SDA = 1;
    IIC_SCL = 1;
}

void IIC_Start(void)
{
    SDA_OUT();
    IIC_SDA = 1;
    IIC_SCL = 1;
    delay_us(4);
    IIC_SDA = 0;
    delay_us(4);
    IIC_SCL = 0;
}

void IIC_Stop(void)
{
    SDA_OUT();
    IIC_SCL = 0;
    IIC_SDA = 0;
    delay_us(4);
    IIC_SCL = 1;
    IIC_SDA = 1;
    delay_us(4);
}

u8 IIC_Wait_Ack(void)
{
    u8 i = 0;
    SDA_IN();
    IIC_SDA = 1;
    delay_us(1);
    IIC_SCL = 1;
    delay_us(1);
    while (READ_SDA)
    {
        i++;
        if (i > 250)
        {
            IIC_Stop();
            return 1;
        }
    }
    IIC_SCL = 0;
    return 0;
}

void IIC_Ack(void)
{
    SDA_OUT();
    IIC_SCL = 0;
    IIC_SDA = 0;
    delay_us(2);
    IIC_SCL = 1;
    delay_us(2);
    IIC_SCL = 0;
}

void IIC_NAck(void)
{
    SDA_OUT();
    IIC_SCL = 0;
    IIC_SDA = 1;
    delay_us(2);
    IIC_SCL = 1;
    delay_us(2);
    IIC_SCL = 0;
}

void IIC_Send_Byte(u8 txd)
{
    u8 t;
    SDA_OUT();
    IIC_SCL = 0;
    for (t = 0; t < 8; t++)
    {
        IIC_SDA = (txd & 0x80) >> 7;
        txd <<= 1;
        delay_us(2);
        IIC_SCL = 1;
        delay_us(2);
        IIC_SCL = 0;
        delay_us(2);
    }
}

u8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    SDA_IN();
    for (i = 0; i < 8; i++)
    {
        IIC_SCL = 0;
        delay_us(2);
        IIC_SCL = 1;
        receive <<= 1;
        if (READ_SDA)
            receive++;
        delay_us(1);
    }
    if (!ack)
        IIC_NAck();
    else
        IIC_Ack();
    return receive;
}












