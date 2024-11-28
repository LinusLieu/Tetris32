#include "stm32f10x.h"
#include "IERG3810_Clock.h"
#include "IERG3810_LED.h"
#include "Global.h"
#include "IERG3810_interrupt.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_Draw.h"
#include "Tetris32_AutoDrop.h"
#include "Tetris32_CheckBlock.h"
#include "Tetris32_random_block_generator.h"
#include "Tetris32_SRS.h"
#include "Tetris32_Joypad.h"
#include "Tetris32_Timer.h"
#include "Tetris32_Preview.h"
#include "IERG3810_USART.h"

/*colors
Z:red��0XF800
L:orange��	0XFD20
O:yellow��0XFFE0
S:green��	0X400
I:cyan��	0X7FF
J:blue��0X1F
T:violet�
���	0X8010
*/

void Delay(u32 count)
{
	u32 i;
	for (i=0;i<count;i++);
}

//For autodrop
void TIM3_IRQHandler(void)
{
	if(TIM3->SR & 1<<0)				//check UIF, RM0008 page-410
	{
		
		IERG3810_DS0(1);
		block_pos_y_movement = -1;
		IERG3810_DS0(0);
		
	}
	TIM3->SR &= ~(1<<0);			//clear UIF, RM0008 page-410
	
}

//For nothing
void TIM4_IRQHandler(void)
{
	if(TIM4->SR & 1<<0)				//check UIF, RM0008 page-410
	{

	}
	TIM4->SR &= ~(1<<0);			//clear UIF, RM0008 page-410
}

void IERG3810_SYSTICK_Init10ms(void)
{
	SysTick->CTRL = 0;	//clear
	SysTick->LOAD = 8999;	//What should be filled? Refer to DDI-0337E page 8-10
	SysTick->VAL = 0;
	SysTick->CTRL |= 0x00000003;	//What should be filled? set internal clock, use interrupt, start count
}

void EXTI4_IRQHandler(void){
	block_pos_x_movement_tmp = 1;
	thread = 4;
	EXTI->PR = 1 << 4;	//Clear this exception pending bit
	cnt += 1;
}

void EXTI2_IRQHandler(void){
	block_pos_x_movement_tmp = -1;
	thread = 4;
	EXTI->PR = 1 << 2;	//Clear this exception pending bit Delay(100000);
	cnt += 3;
}

void EXTI0_IRQHandler(void)
{
	EXTI->PR = 0x00000001;	//Clear this exception pending bit
}

void EXTI9_5_IRQHandler(void)
{
	joypadkey[key_count] = GPIOC->IDR & 1 << 9;
	key_count++;
	Delay(1);
	EXTI->PR = 1<<8;
}

void EXTI15_10_IRQHandler(void)
{
	joypadkey[key_count] = GPIOC->IDR & 1 << 9;
	key_count++;
	Delay(1);
	EXTI->PR = 1<<12;
}


int main(void)
{
	int i = 0, j = 0;
	u8 tmp = 0,tmp2 = 0,tmp3 = 0;
	u8 playstate = 0;
	u8 state_new = 0,game_new = 1;
	u8 arror_light = 1;
	
	direction = 0;
	DAS = 20;
	block_generate_pos_x = 4;
	block_generate_pos_y = 19;
	ASP = 20;
	state = 0;
	gamemode = 0;

	block_pos_x = block_generate_pos_x;
	block_pos_y = block_generate_pos_y;

	shift_delay = 30;
	shift_DAS = 200 / DAS;

	IERG3810_SYSTICK_Init10ms();
	IERG3810_clock_tree_init();
	IERG3810_TFTLCD_Init();
	IERG3810_NVIC_SetPriorityGroup(5);
	IERG3810_key0_ExtiInit();
	IERG3810_key2_ExtiInit();
	IERG3810_keyUP_ExtiInit();
	IERG3810_LED_Init();
	IERG3810_USART2_init(36,9600);
	
	Joypad_init();
	Joypad_Latch_ExtiInit();
	Joypad_Clock_ExtiInit();
	

	//IERG3810_TIM3_Init(14399 / DAS * 10,7199);
	//IERG3810_TIM4_Init(5000,7199);
	
	Delay(1000000);
	IERG3810_TFTLCD_FillRectangle(0x0000,0,240,0,320);
	Delay(1000000);
	IERG3810_TFTLCD_FillRectangle(0x0000,0,240,0,320);

	while(1)
	{
	//IERG3810_TFTLCD_ShowChar(0,300,state + 48,0xFFFF,0x0000);
	//IERG3810_TFTLCD_ShowChar(10,300,tmp2 + 48,0xFFFF,0x0000);
	cnt++;
	if(state == 0){
		//For Default menu
		if(state_new == 0){
			IERG3810_TFTLCD_FillRectangle(0x0000,0,240,0,320);
			Delay(1000000);
			Draw_Tetris_Letters();
			Draw_Menu_Arror(1,gamemode);
			for(i = 4; i < 14; i++)
			{
				for(j = 4; j < 24; j++)
				{
					Playfield[i][j] = 0;
				}
			}
			state_new = 1;
			key_start = 0;
			state = 0;
		}
		if(key_select){
			gamemode = (gamemode + 1) % 4;
			Draw_Menu_Arror(1,gamemode);
			key_select = 0;
		}
		if(key_start){
			tmp2 = 0;
			USART_print(2,"Gamestart!");
			if(gamemode == 2){
				key_start = 0;
				state = 6;
			}else if(gamemode == 3){
				key_start = 0;
				state = 7;
			}else{
				key_start = 0;
				state = 3;	//goto counting down
				arror_light = 0;
				IERG3810_DS1(0);
				IERG3810_TFTLCD_FillRectangle(0x0000,0,240,0,320);
				Delay(2000000);
				TimerHeartBeat = 1000;
			}
			state_new = 0;
			game_new = 1;
			
		}
		if(task1HeartBeat > 200){
			arror_light = (arror_light + 1) % 2;
			Draw_Menu_Arror(arror_light,gamemode);
			task1HeartBeat = 0;
			IERG3810_DS1(arror_light);
		}
		


		
	}else if(state == 1){
		if(game_new){
			IERG3810_TFTLCD_FillRectangle(0x0000,0,240,0,320);
			Delay(1000000);
			Playfield_init();
			tmp2 = 0;
			DAS_Timer = 0;
			ARR_Timer = 0;
			autoDrop = 0;
			direction = 0;
			block_center_x = 1;
			block_center_y = 2;
			block_pos_x = block_generate_pos_x;
			block_pos_y = block_generate_pos_y;
			block_pos_x_movement = 0;
			block_pos_y_movement = 0;
			block_pos_x_pre = 0;
			block_pos_y_pre = 0;
			block_pos_x_movement_tmp = 0;
			block_pos_y_movement_tmp = 0;;
			block_center_offset_x=0;
			block_center_offset_y=0;
			cnt = 0;
			key = 0;
			line_count = 0;
			help = 0;
			score = 0;
			Bag7cnt = 0;
			generate_7bag();
			random_block_generator();
			Delay(1000000);
			Draw_playfield();
			Delay(1000000);
			Draw_Preview_init();
			Delay(1000000);
			Draw_block();
			Draw_Preview();
			game_new = 0;
			line_count = 0;
			if(gamemode == 1){
				Draw_linecount_40();
				Draw_40line();
				USART_print(2,"You choosed 40line mode.");
			}else{
				Draw_linecount();
				Draw_blitz();
				USART_print(2,"You choosed Blitz mode.");
			}
			TimerHeartBeat = 0;
		}
		if(gamemode == 1){
			//For 40 Lines
			if(line_count >= 40){
				state = 4;	//win page
				score = TimerHeartBeat;
			}
			
		}
		if(gamemode == 0){
			//For Blitz
			if(TimerHeartBeat >= 120000){ //120000
				state = 5;	//Blitz end page
				score = line_count;
			}
		}
		if (task1HeartBeat % 50 == 0)
			{
				
				if(block_pos_y_movement != 0){
					tmp = Bottom_check_conv();
					Draw_update();

				}

				if(block_pos_x_movement_tmp != 0){
					tmp = Shift_check();
					Draw_update();
					
				}

				Draw_Timer();
			}
		
		if(task1HeartBeat % 100 == 0){
					
					task1HeartBeat = 0;
					//Draw_update();
		}
	}else if(state == 2){
		//For death page
		if(tmp2 == 0){
			USART_print(2,"You are dead!");
			Draw_deathpage();
			game_new = 1;
			state_new = 0;
			gamemode = 0;
			tmp2 = 1;
			cnt = cnt / 7;
			temp = 0;
		}
	}else if(state == 3){
		//counting down
		if(TimerHeartBeat >= 3990){
			state = 1;
			IERG3810_TIM3_Init(14399 / DAS * 10,7199);
			TIM3->CNT = 0;
			continue;
		}
		if(TimerHeartBeat % 1000 == 0){
			IERG3810_TFTLCD_FillRectangle(0x0000,0,240,0,320);
			Delay(100000);
			//IERG3810_TFTLCD_FillRectangle(0x0000,82,75,90,140);
			Delay(100);
			IERG3810_TFTLCD_SevenSegment(0xFFFF,82,90,4 - TimerHeartBeat/1000);
		}
	}else if(state == 4){
		//For win the 40 lines
		if(tmp2 == 0){
			USART_print(2,"You win!");
			Draw_end_40line();
			game_new = 1;
			state_new = 0;
			gamemode = 0;
			tmp2 = 1;
		}
		
	}else if(state == 5){
		//For time end of Blitz
		if(tmp2 == 0){
			USART_print(2,"Oh no... Time is up.");
			Draw_end_Blitz();
			game_new = 1;
			state_new = 0;
			gamemode = 0;
			tmp2 = 1;
		}

	}else if(state == 6){
		//Help page
		IERG3810_TFTLCD_FillRectangle(0x0000,0,240,0,320);
		Delay(1000000);
		Draw_help_page();
		Delay(30000000);
		state = 0;
		help = 0;
	}else if(state == 7){
		//Help page
		IERG3810_TFTLCD_FillRectangle(0x0000,0,240,0,320);
		Delay(1000000);
		Draw_name_page();
		Delay(30000000);
		state = 0;
		help = 0;
	}


	//Call Joypad

	if(task2HeartBeat > 5){
		Joypad_sendpulse();
		Joypad_input_recog();
		task2HeartBeat = 0;
	}
	}
}
