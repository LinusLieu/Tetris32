#include "stm32f10x.h"
#include "IERG3810_Buzzer.h"

// put your procedure and code here
void IERG3810_Buzzer_Init(void){
	RCC->APB2ENR |= 0x00000008;	//ENABLE APB2 A,B,E CLOCK
	
	GPIOB->CRH &= 0xFFFFFFF0;	//init Buzzer
	GPIOB->CRH |= 0x00000003;
	GPIOB->BRR &= 0xFFFFFEFF;
}
