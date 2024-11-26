#include "stm32f10x.h"
#include "Global.h"
#include "Tetris32_Joypad.h"
#include "Tetris32_SRS.h"

void Joypad_init(void){
    RCC->APB2ENR |= 1 << 4;
    GPIOC->CRH &= 0xFFF0FF00;
    GPIOC->CRH |= 0x00030083;
    GPIOC->ODR &= 0xFFFFFDFF;
    GPIOC->ODR |= 0x00000200;

    /*
    //These part init for v3 Board

    RCC->APB2ENR |= 0x00000024;
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
    u8 LED_temp = 0;
    u8 tmp;

    //Key 7 for right
    //Key 6 for left
    //key 5 for down
    //key 4 for up


    //To show if there has any key been pressed though DS1
    for(i = 0;i<8;i++){
        if(pressed[i])
        LED_temp += pressed[i];
    }
    IERG3810_DS1(LED_temp);
    
    //At death or end page
    if(state == 2 || state == 4 || state == 5){
        for(i = 0;i<7;i++){
            if(!joypadkey[i]){
                state = 0;
            }
        }
    }else if(state == 1){
        //Key 7 for moving left
        if(!joypadkey[6]){
            if(pressed[6] == 0){
            block_pos_x_movement_tmp = -1;
            pressed[6] = 1;
            }else{
                shift_delay_cnt++;
            }
            if(shift_delay_cnt >= shift_delay){
                if(shift_DAS_cnt > shift_DAS){
                    block_pos_x_movement_tmp = -1;
                    shift_DAS_cnt = 0;
                }else{
                    shift_DAS_cnt++;
                }
                shift_delay_cnt = shift_delay;
            }
        }else{
            if(pressed[6]){
                shift_delay_cnt = 0;
                shift_DAS_cnt = 0;
                pressed[6] = 0;
            }
            
            }

        //Key 8 for moving right
        if(!joypadkey[7]){
        if(pressed[7] == 0){
            block_pos_x_movement_tmp = 1;
            pressed[7] = 1;
            }else{
                shift_delay_cnt++;
            }
            if(shift_delay_cnt >= shift_delay){
                if(shift_DAS_cnt > shift_DAS){
                    block_pos_x_movement_tmp = 1;
                    shift_DAS_cnt = 0;
                }else{
                    shift_DAS_cnt++;
                }
                shift_delay_cnt = shift_delay;
            }
        }else{
            if(pressed[7]){
                shift_delay_cnt = 0;
                shift_DAS_cnt = 0;
                pressed[7] = 0;
            }
            
            }

        //for softdrop
        if(!joypadkey[5]){
            if(pressed[5] == 0){
            //block_pos_y_movement = -1;  //For debug , moving down
            IERG3810_TIM3_NewARR(14399 / ASP);	
            pressed[5] = 1;
            }
        }else{
            if(pressed[5] == 1){

            IERG3810_TIM3_NewARR(14399  / DAS * 10);	
            pressed[5] = 0;
            }
            
        }


        if(!joypadkey[4]){
            if(pressed[4] == 0){
            //block_pos_y_movement = 1;   //For debug , moving up
            //IERG3810_TIM3_NewARR(14399 / ASP);	
            pressed[4] = 1;
            }
        }else{
            if(pressed[4] == 1){

            //IERG3810_TIM3_NewARR(14399  / DAS * 10);	
            pressed[4] = 0;
            }
            
        }

        //Key 1 for rotate clockwise
        if(!joypadkey[0]){
            if(pressed[0] == 0){
            rotate_clockwise();
            pressed[0] = 1;
            }
        }else{
            if(pressed[0] == 1){
            pressed[0] = 0;
            }
            
        }

        //Key 2 for rotate anti-clockwise
        if(!joypadkey[1]){
            if(pressed[1] == 0){
            rotate_anticlockwise();
            pressed[1] = 1;
            }
        }else{
            if(pressed[1] == 1){
            pressed[1] = 0;
            }
            
        }
    }else
    

    //At menu page
    if(state == 0){
        //Key 3 for select
        if(!joypadkey[2]){
            if(pressed[2] == 0){
            key_select = 1;
            pressed[2] = 1;
            }
        }else{
            if(pressed[2] == 1){
            pressed[2] = 0;
            }
            
        }

        //Key 4 for start
        if(!joypadkey[3]){
            if(pressed[3] == 0){
            key_start = 1;
            pressed[3] = 1;
            }
        }else{
            if(pressed[3] == 1){
            pressed[3] = 0;
            }
            
        }
    }
   
}

void Joypad_sendpulse(void){
    u8 i;
	key_count = 0;
	GPIOC->BSRR |= 1 << 8;
	Delay(1);
	GPIOC->BRR |= 1 << 8;
	Delay(1);
	for(i = 0;i<8;i++){
		GPIOC->BSRR |= 1 << 12;
		Delay(1);
		GPIOC->BRR |= 1 << 12;
		Delay(1);
	}
}


