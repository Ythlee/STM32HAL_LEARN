#include "dma.h"
#include "led.h"
#include "delay.h"
#include "usart.h"

DMA_HandleTypeDef DMA_Handle;
DMA_HandleTypeDef USART1_Handle;
const u32 aSRC_Const_Buffer[BUFFER_SIZE] = {
    0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
    0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
    0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
    0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
    0x41424344, 0x44564748, 0x494A4B4C, 0x4D4E4F50,
    0x51525345, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
    0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
    0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80};

u32 aDST_Buffer[BUFFER_SIZE];

void DMA_Config(DMA_Channel_TypeDef *chx)
{
    HAL_StatusTypeDef DMA_Status = HAL_ERROR;
    __HAL_RCC_DMA1_CLK_ENABLE();

    DMA_Handle.Instance = chx;
    DMA_Handle.Init.Direction = DMA_MEMORY_TO_MEMORY;
    DMA_Handle.Init.MemInc = DMA_MINC_ENABLE;
    DMA_Handle.Init.PeriphInc = DMA_PINC_ENABLE;
    DMA_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    DMA_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    DMA_Handle.Init.Mode = DMA_NORMAL;
    DMA_Handle.Init.Priority = DMA_PRIORITY_MEDIUM;

    HAL_DMA_Init(&DMA_Handle);

    DMA_Status = HAL_DMA_Start(&DMA_Handle, (u32)aSRC_Const_Buffer, (u32)aDST_Buffer, BUFFER_SIZE);
    if (DMA_Status != HAL_OK)
    {
        while (1)
        {
            LED0 = !LED0;
            delay_ms(500);
        }
    }
}

u8 Buffercmp(const u32 *aSRC_Const_Buffer, u32 *aDST_Buffer, u32 bufflenth)
{
    while (bufflenth--)
    {
        if (*aSRC_Const_Buffer != *aDST_Buffer)
        {
            return 0;
        }
        aSRC_Const_Buffer++;
        aDST_Buffer++;
    }
    return 1;
}


void USART1_DMA_Config(void)
{
    __HAL_RCC_DMA1_CLK_ENABLE();
    
    USART1_Handle.Instance = DMA1_Channel4;
    USART1_Handle.Init.Direction = DMA_MEMORY_TO_PERIPH;
    USART1_Handle.Init.MemInc = DMA_MINC_ENABLE;
    USART1_Handle.Init.PeriphInc = DMA_PINC_DISABLE;
    USART1_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    USART1_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    USART1_Handle.Init.Mode = DMA_NORMAL;
    USART1_Handle.Init.Priority = DMA_PRIORITY_MEDIUM;
    

    HAL_DMA_Init(&USART1_Handle);
    __HAL_LINKDMA(&UART1_Handler,hdmatx,USART1_Handle);
    
}








