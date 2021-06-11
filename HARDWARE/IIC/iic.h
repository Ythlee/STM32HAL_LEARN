#ifndef __IIC_H_
#define __IIC_H_

#include "sys.h"

#define SDA_IN()                    \
    {                               \
        GPIOB->CRH &= 0xFFFF0FFF;   \
        GPIOB->CRH |= (u32)8 << 12; \
    }

#define SDA_OUT()                   \
    {                               \
        GPIOB->CRH &= 0xFFFF0FFF;   \
        GPIOB->CRH |= (u32)3 << 12; \
    }

#define IIC_SCL PBout(10)
#define IIC_SDA PBout(11)
#define READ_SDA PBin(11)

void IIC_Init(void);
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_Send_Byte(u8 txd);
u8 IIC_Read_Byte(unsigned char ack);

#endif // !__IIC_H_
