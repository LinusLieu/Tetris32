#ifndef __TETRIS32_TIMER_H
#define __TETRIS32_TIMER_H
#include "Global.h"
#include "stm32f10x.h"

void IERG3810_TIM3_Init(u16 arr,u16 psc);
void IERG3810_TIM4_Init(u16 arr,u16 psc);
#endif