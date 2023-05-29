#ifndef __TIMER_H
#define __TIMER_H
#include "stm32f10x.h"

void TIM6_init(u16 arr,u16 psc);
void TIM6_IRQHandler(void);

#endif

