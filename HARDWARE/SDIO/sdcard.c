#include "sdcard.h"
#include "stdio.h"

SD_HandleTypeDef SD_Handle;
HAL_SD_CardInfoTypeDef SDCardInfo;
u8 SD_Card_Init(void)
{
    HAL_StatusTypeDef SD_STAT;
    SD_Handle.Instance = SDIO;
    SD_Handle.Init.ClockEdge = SDIO_CLOCK_EDGE_RISING;
    SD_Handle.Init.ClockBypass = SDIO_CLOCK_BYPASS_DISABLE;
    SD_Handle.Init.ClockDiv = SDIO_TRANSFER_CLK_DIV;
    SD_Handle.Init.ClockPowerSave = SDIO_CLOCK_POWER_SAVE_DISABLE;
    SD_Handle.Init.BusWide = SDIO_BUS_WIDE_1B;
    SD_Handle.Init.HardwareFlowControl = SDIO_HARDWARE_FLOW_CONTROL_DISABLE;

    SD_STAT = HAL_SD_Init(&SD_Handle);
    if (SD_STAT != HAL_OK)
        return 1;
    SD_STAT = HAL_SD_ConfigWideBusOperation(&SD_Handle, SDIO_BUS_WIDE_4B);
    if (SD_STAT != HAL_OK)
        return 2;

    return 0;
}
void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
    DMA_HandleTypeDef TxDMAHandle, RxDMAHandle;
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_SDIO_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_DMA2_CLK_ENABLE();

    GPIO_Initure.Mode = GPIO_MODE_AF_PP;
    GPIO_Initure.Pin = GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_2;
    HAL_GPIO_Init(GPIOD, &GPIO_Initure);
}

u8 SD_GetCardInfo(HAL_SD_CardInfoTypeDef *cardinfo)
{
    u8 SD_STAT;
    SD_STAT = HAL_SD_GetCardInfo(&SD_Handle, cardinfo);
    return SD_STAT;
}

u8 SD_GetCardState(void)
{
    return ((HAL_SD_CARD_READY == HAL_SD_GetCardState(&SD_Handle)) ? SD_TRANSFER_OK : SD_TRANSFER_BUSY);
}

void show_sdcard_info(void)
{
    uint64_t CardCap;
    HAL_SD_CardCIDTypeDef SDCard_CID;
    HAL_SD_GetCardCID(&SD_Handle, &SDCard_CID);
    SD_GetCardInfo(&SDCardInfo);

    switch (SDCardInfo.CardType)
    {
    case CARD_SDSC:
        if (SDCardInfo.CardVersion == CARD_V1_X)
            printf("Card Type:SDSC V1\r\n");
        else if (SDCardInfo.CardVersion == CARD_V2_X)
            printf("Card Type:SDSC V2\r\n");
        break;
    case CARD_SDHC_SDXC:
        printf("Card Type:SDHC\r\n");
        break;
    }

    CardCap = (uint64_t)(SDCardInfo.LogBlockNbr)*(uint64_t)(SDCardInfo.LogBlockSize);
    printf("Card ManufacturerID:%d\r\n",SDCard_CID.ManufacturerID);
 	printf("Card RCA:%d\r\n",SDCardInfo.RelCardAdd);
	printf("LogBlockNbr:%d \r\n",(u32)(SDCardInfo.LogBlockNbr));
	printf("LogBlockSize:%d \r\n",(u32)(SDCardInfo.LogBlockSize));
	printf("Card Capacity:%d MB\r\n",(u32)(CardCap>>20));
 	printf("Card BlockSize:%d\r\n\r\n",SDCardInfo.BlockSize);
}

