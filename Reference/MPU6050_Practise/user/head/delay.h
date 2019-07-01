#ifndef DELAY_H
#define DELAY_H
#include "stm32f10x.h"
#define BIT(n) (u32)(0x01<<n)

void Delay_Init (void);
void Delay_us (u32 time_us);
void Delay_ms(u32 time_ms);
void RCC_Configuration (void);

#endif
