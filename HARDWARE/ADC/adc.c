#include "adc.h"
#include "delay.h"
ADC_HandleTypeDef ADC_Handle;

void ADC_Init(void)
{
    RCC_PeriphCLKInitTypeDef ADC_CLK;
    ADC_CLK.PeriphClockSelection = RCC_PERIPHCLK_ADC;
    ADC_CLK.AdcClockSelection = RCC_ADCPCLK2_DIV6;
    HAL_RCCEx_PeriphCLKConfig(&ADC_CLK);

    ADC_Handle.Instance = ADC1;
    ADC_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    ADC_Handle.Init.ScanConvMode = DISABLE;
    ADC_Handle.Init.ContinuousConvMode = DISABLE;
    ADC_Handle.Init.NbrOfConversion = 1;
    ADC_Handle.Init.DiscontinuousConvMode = DISABLE;
    ADC_Handle.Init.NbrOfDiscConversion = 0;
    ADC_Handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    HAL_ADC_Init(&ADC_Handle);
    HAL_ADCEx_Calibration_Start(&ADC_Handle);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOA_CLK_ENABLE();
    GPIO_Initure.Mode = GPIO_MODE_ANALOG;
    GPIO_Initure.Pin = GPIO_PIN_1;
    GPIO_Initure.Pull = GPIO_NOPULL;
    GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);
}

u16 Get_Adc(u32 ch)
{
    ADC_ChannelConfTypeDef ADC1_ChanConf;

    ADC1_ChanConf.Channel = ch;
    ADC1_ChanConf.Rank = 1;
    ADC1_ChanConf.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
    HAL_ADC_ConfigChannel(&ADC_Handle, &ADC1_ChanConf);
    HAL_ADC_Start(&ADC_Handle);
    HAL_ADC_PollForConversion(&ADC_Handle, 10);
    return (u16)HAL_ADC_GetValue(&ADC_Handle);
}

u16 Get_Average(u32 ch, u8 times)
{
    u32 temp_val = 0;
    u8 t;
    for (t = 0; t < times; t++)
    {
        temp_val += Get_Adc(ch);
        delay_ms(5);
    }
    return temp_val / times;
}
