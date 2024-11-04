#include "stm32f10x.h"
#include "IERG3810_LED.h"

// put your procedure and code here
void IERG3810_LED_Init(void){
	RCC->APB2ENR |= 0x00000048;	//ENABLE APB2 B,E CLOCK
	
	GPIOB->CRL &= 0xFF0FFFFF;	//init DS0
	GPIOB->CRL |= 0x00300000;
	GPIOE->CRL &= 0xFF0FFFFF;	//init DS1
	GPIOE->CRL |= 0x00300000;
	
	GPIOB->BSRR = 1 << 5;	//turn off DS0,DS1
	GPIOE->BSRR = 1 << 5;
}

void IERG3810_DS0(u8 i)
{
	if(i){
		GPIOB->BRR = 1 << 5;
	}else{
		GPIOB->BSRR = 1 << 5;
	}
}

void IERG3810_DS1(u8 i)
{
	if(i){
		GPIOE->BRR = 1 << 5;
	}else{
		GPIOE->BSRR = 1 << 5;
	}
}
