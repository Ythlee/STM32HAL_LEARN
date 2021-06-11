#ifndef __DMA_H_
#define __DMA_H_

#include "sys.h"

#define BUFFER_SIZE 32
extern const u32 aSRC_Const_Buffer[BUFFER_SIZE];
extern u32 aDST_Buffer[BUFFER_SIZE];
extern DMA_HandleTypeDef DMA_Handle;
extern DMA_HandleTypeDef USART1_Handle;
void DMA_Config(DMA_Channel_TypeDef *chx);
u8 Buffercmp(const u32* aSRC_Const_Buffer, u32* aDST_Buffer, u32 bufflenth);

void USART1_DMA_Config(void);

void MX_DMA_Init(void);
#endif // !__DMA_H_
