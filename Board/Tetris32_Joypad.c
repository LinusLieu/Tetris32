#include "stm32f10x.h"
#include "Global.h"
#include "Tetris32_Joypad.h"

void Joypad_init(void){
    RCC->APB2ENR |= 1 << 4;
    GPIOC->CRH &= 0xFFF0FF00;
    GPIOC->CRH |= 0x00030083;
    GPIOC->ODR &= 0xFFFFFDFF;
    GPIOC->ODR |= 0x00000200;
    
    /*RCC->APB2ENR |= 0x00000024;
    GPIOB->CRH &= 0xFFFF00FF;
    GPIOB->CRH |= 0xFFFF38FF;
    GPIOD->CRL &= 0xFFFF0FFF;
    GPIOD->CRL |= 0x00003000;

    GPIOB->ODR &= 0xFFFFFDFF;
    GPIOB->ODR |= 0x00000400;*/
}

void Joypad_Latch_ExtiInit(void)
{
	//at PC8, EXTI-8, IRQ#23
	RCC->APB2ENR |= 1 << 4;		//enable port-C clock for Joypad
	RCC->APB2ENR |= 0x01;				// ??RM0008, page146) Enable AFIO clock
	AFIO->EXTICR[2] &= 0xFFFFFFF0;	//	(RM0008, AFIO_EXTICR1, page-191)
	AFIO->EXTICR[2] |= 0x00000002;
	EXTI->IMR				|= 1 << 8;	//(RM0008,page-211) edge trigger
	EXTI->FTSR			|= 1 << 8;	//(RM0008,page-212) falling edge
	//EXTI->RTST		|= 1 << 2;	//(RM0008,page-212) rising edge
	NVIC->IP[8] = 0x65;					//set priority of this interrupt
	NVIC->ISER[0] &= ~(1<<23);		//set NVIC 'SET ENABLE REGISTER'
															//DDI0337E page-8-3
	NVIC->ISER[0] |= (1<<23);		//IRQ23
}

void Joypad_Clock_ExtiInit(void)
{
	//at PC12, EXTI-12, IRQ#40	
	RCC->APB2ENR |= 1 << 4;		//enable port-C clock for Joypad
	RCC->APB2ENR |= 0x01;				// ??RM0008, page146) Enable AFIO clock
	AFIO->EXTICR[3] &= 0xFFFFFFF0;	//	(RM0008, AFIO_EXTICR1, page-191)
	AFIO->EXTICR[3] |= 0x00000002;
	EXTI->IMR				|= 1 << 12;	//(RM0008,page-211) edge trigger
	EXTI->FTSR			|= 1 << 12;	//(RM0008,page-212) falling edge
	//EXTI->RTST		|= 1 << 2;	//(RM0008,page-212) rising edge
	NVIC->IP[8] = 0x65;					//set priority of this interrupt
	NVIC->ISER[1] &= ~(1<<8);		//set NVIC 'SET ENABLE REGISTER'
															//DDI0337E page-8-3
	NVIC->ISER[1] |= (1<<8);		//IRQ40
}


void Joypad_input_recog(void){
    u8 i;
    if(!joypadkey[6]){
        IERG3810_DS0(1);
        Delay(1000);
        block_pos_x_movement_tmp = -1;
	    Shift_check();
        IERG3810_DS0(0);
    }
    if(!joypadkey[0]){
        thread = 7;
    }
    /*
    if(!joypadkey[6]){
        if(!joypad_timer){
        block_pos_x_movement_tmp = -1;
        joypad_timer++;
	    thread = 4;
        }else if(joypad_timer > joypad_timer_max){
            if(!(ARR_Timer % 20)){
                block_pos_x_movement_tmp = -1;
	            thread = 4;
            }
            ARR_Timer++;
        }
    }else{
        joypad_timer = 0;
        ARR_Timer = 0;
    }
    if(!joypadkey[7]){
        if(!joypad_timer){
        block_pos_x_movement_tmp = 1;
        joypad_timer++;
	    thread = 4;
        }else if(joypad_timer > joypad_timer_max){
            if(!(ARR_Timer % 20)){
                block_pos_x_movement_tmp = 1;
	            thread = 4;
            }
            ARR_Timer++;
        }
    }else{
        joypad_timer = 0;
        ARR_Timer = 0;
    }
    */
}

void Joypad_sendpulse(void){
    u8 i;
	key_count = 0;
	GPIOC->BSRR |= 1 << 8;
	Delay(1 );
	GPIOC->BRR |= 1 << 8;
	Delay(1);
	for(i = 0;i<8;i++){
		GPIOC->BSRR |= 1 << 12;
		Delay(1);
		GPIOC->BRR |= 1 << 12;
		Delay(1);
	}
}


