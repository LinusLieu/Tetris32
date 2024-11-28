#include "stm32f10x.h"
#include "IERG3810_LED.h"

void IERG3810_NVIC_SetPriorityGroup (u8 prigroup)
{
	//Set PRIGROUP AIRCR[10:8]
	u32 temp, temp1;
	temp1 = prigroup & 0x00000007;
	temp1 <<= 8;
	temp = SCB->AIRCR;
	temp &= 0x0000F8FF;
	temp |= 0x05FA0000;
	temp |= temp1;
	SCB->AIRCR = temp;
}

void IERG3810_key2_ExtiInit(void)
{
	//KEY2 at PE2, EXTI-2, IRQ#8
	RCC->APB2ENR |= 1 <<6;
	GPIOE->CRL &= 0xFFFFF0FF;
	GPIOE->CRL |= 0x00000800;
	GPIOE->ODR |= 1 << 2;
	RCC->APB2ENR |= 0x01;
	AFIO->EXTICR[0] &= 0xFFFFF0FF;
	AFIO->EXTICR[0] |= 0x00000400;
	EXTI->IMR				|= 1 << 2;
	EXTI->FTSR			|= 1 << 2;
	
	NVIC->IP[8] = 0x65;
	NVIC->ISER[0] &= ~(1<<8);
	NVIC->ISER[0] |= (1<<8);
}

void IERG3810_key0_ExtiInit(void)
{
    // KEY0 at PE4, EXTI-4, IRQ#10
    RCC->APB2ENR |= 1 << 6;
    GPIOE->CRL &= 0xFFF0FFFF;
    GPIOE->CRL |= 0x00080000;
    GPIOE->ODR |= 1 << 4;

    RCC->APB2ENR |= 0x01;
    AFIO->EXTICR[1] &= 0xFFFFFFF0;
    AFIO->EXTICR[1] |= 0x00000004;

    EXTI->IMR |= 1 << 4;
    EXTI->FTSR |= 1 << 4;

    NVIC->IP[10] = 0x65;
    NVIC->ISER[0] &= ~(1 << 10);
    NVIC->ISER[0] |= (1 << 10);
}


void IERG3810_keyUP_ExtiInit(void)
{
	//KEYUP at PA0, EXTI-0, IRQ#8
	RCC->APB2ENR |= 1 << 2;
	GPIOA->CRL &= 0xFFFFFFF0;
	GPIOA->CRL |= 1 << 3;
	GPIOA->ODR &= 0xFFFFFFFE;
	RCC->APB2ENR |= 0x01;
	AFIO->EXTICR[0] &= 0xFFFFFFF0;
	EXTI->IMR				|= 0x00000001;
	EXTI->RTSR		|= 0x00000001;
	
	NVIC->IP[6] = 0x35;	
	NVIC->ISER[0] &= ~(1<<6);
	NVIC->ISER[0] |= (1<<6);
}



void IERG3810_PS2key_ExtiInit(void)
{
	RCC->APB2ENR |= 1 << 4;
	
	GPIOC->CRH &= 0xFFFF00FF;	
	GPIOC->CRH |= 0x00004400;

	RCC->APB2ENR |= 0x01;
	AFIO->EXTICR[2] &= 0xFFFF0FFF;
	AFIO->EXTICR[2] |= 0x00002000;
	EXTI->IMR				|= 1 << 11	;
	EXTI->FTSR			|= 1 << 11;	

	
	NVIC->IP[8] = 0x35;
	NVIC->ISER[1] &= ~(1<<8);
	NVIC->ISER[1] |= (1<<8);
	
}
