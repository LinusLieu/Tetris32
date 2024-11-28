#include "stm32f10x.h"
#include "IERG3810_KEY.h"
#include "global.h"

//Init setting for Key 1,2,4 and UP
void IERG3810_KEY_Init(void){
	RCC->APB2ENR |= 0x00000044;	//ENABLE APB2 A,E CLOCK
	
	GPIOE->CRL &= 0xFFF000FF;	//init Key1,Key2,Key 4
	GPIOE->CRL |= 0x00088800;
	GPIOE->ODR &= 0xFFFFFFE3;
	GPIOE->ODR |= 0x0000001C;
	
	GPIOA->CRL &= 0xFFFFFFF0;	//init Key_UP
	GPIOA->CRL |= 1 << 3;
	GPIOA->ODR &= 0xFFFFFFFE;
}
