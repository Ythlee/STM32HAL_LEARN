#ifndef ADC__H_
#define ADC__H_

#include "sys.h"

void ADC_Init(void);
u16 Get_Average(u32 ch, u8 times);

#endif // !ADC__H_
