#include "stm32f10x.h"
#include "IERG3810_KEY.h"
#include "global.h"

// put your procedure and code here
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

/*
void IERG3810_PS2key(u8 stat,u8 key){
	if(stat){
		switch(key){
			case 0x69:
				//USART_print_int(2,0xFF);
				IERG3810_DS0(0);
				break;
			case 0x72:
				IERG3810_DS1(0);
				break;
			case 0x6B:
				//USART_print_int(2,0xEE);
				IERG3810_DS0(0);
				break;
			case 0x73:
				IERG3810_DS1(0);
				break;
			default:
				IERG3810_PS2key_reset();
			}
	}else{
		switch(key){
			case 0x69:
				//USART_print_int(2,0xEE);
				IERG3810_DS0(0);
				IERG3810_DS0(1);
				break;
			case 0x72:
				IERG3810_DS1(0);
				IERG3810_DS1(1);
				break;
			case 0x6B:
				IERG3810_DS0(0);
				Delay(100000);
				IERG3810_DS0(1);
				break;
			case 0x73:
				IERG3810_DS1(0);
				Delay(100000);
				IERG3810_DS1(1);
				break;
			default:
				IERG3810_PS2key_reset();
			}
	}
}
*/