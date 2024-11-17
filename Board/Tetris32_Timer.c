#include "stm32f10x.h"
#include "Global.h"
#include "Tetris32_Timer.h"

void IERG3810_TIM3_Init(u16 arr,u16 psc)
{
	//TIM3, IRQ#29
	RCC->APB1ENR |= 1<<1;					//RM0008 page-115, refer to lab-1
	TIM3->ARR = arr;							//RM0008 page-419
	TIM3->PSC = psc;							//RM0008 page-418
	TIM3->DIER |= 1<<0;						//RM0008 page-409
	TIM3->CR1 |= 0X01;						//RM0008 page-404
	NVIC->IP[29] = 0x45;					//refer to lab-4, DDI0337E
	NVIC->ISER[0] |= (1<<29);			//refer to lab-4, DDI0337E
}

void IERG3810_TIM4_Init(u16 arr,u16 psc)
{
	//TIM4, IRQ#30
	RCC->APB1ENR |= 1<<2;					//RM0008 page-115, refer to lab-1
	TIM4->ARR = arr;							//RM0008 page-419
	TIM4->PSC = psc;							//RM0008 page-418
	TIM4->DIER |= 1<<0;						//RM0008 page-409
	TIM4->CR1 |= 0X01;						//RM0008 page-404
	NVIC->IP[30] = 0x45;					//refer to lab-4, DDI0337E
	NVIC->ISER[0] |= (1<<30);			//refer to lab-4, DDI0337E
}
