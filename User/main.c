#include "stm32f10x.h"
#include "IERG3810_USART.h"
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

//include the head file of menu and death page here
//include the head file of menu and death page here
//include the head file of menu and death page here
//include the head file of menu and death page here
//include the head file of menu and death page here
//include the head file of menu and death page here
//include the head file of menu and death page here
//include the head file of menu and death page here
//include the head file of menu and death page here
//include the head file of menu and death page here



/*colors
Z:red��0XF800
L:orange��	0XFD20
O:yellow��0XFFE0
S:green��	0X400
I:cyan��	0X7FF
J:blue��0X1F
T:violet��	0X8010
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
	debug++;
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
	u8 posx[12] = {48,79,83,63,56,0,0,0,0,0,0,0};
	u8 posy[12] = {48,79,83,63,57,0,0,0,0,0,0,0};
	u8 centx[12] = {35,69,78,84,69,82,63,56,0,0,0,0};
	u8 centy[12] = {35,69,78,84,69,82,63,57,0,0,0,0};
	u8 cov[12] = {67,79,78,86,0,0,0,0,0,0,0,0};
	u8 press[12] = {43,69,89,29,0,0,0,0,0,0,0,0};
	u8 tmp,tmp2,tmp3 = 0;
	u8 playstate = 0;
	
	u8 state_new = 0,game_new = 1;

	direction = 0;
	DAS = 20;
	block_generate_pos_x = 4;
	block_generate_pos_y = 18;
	ASP = 20;
	state = 0;
	gamemode = 1;

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
	IERG3810_USART2_init(36,9600);
	IERG3810_LED_Init();
	
	Joypad_init();
	Joypad_Latch_ExtiInit();
	Joypad_Clock_ExtiInit();
	

	//IERG3810_TIM3_Init(14399 / DAS * 10,7199);
	//IERG3810_TIM4_Init(5000,7199);
	
	Delay(1000000);
	IERG3810_TFTLCD_FillRectangle(0x0000,0,240,0,320);
	Delay(1000000);
	IERG3810_TFTLCD_FillRectangle(0x0000,0,240,0,320);
	

	/*
	for(i = 0;i<12;i++)
	{
		IERG3810_TFTLCD_ShowChar(10 * (1+i),300,posx[i]+32,0xFFFF,0x0000);
		IERG3810_TFTLCD_ShowChar(10 * (1+i)+120,300,posy[i]+32,0xFFFF,0x0000);
		IERG3810_TFTLCD_ShowChar(10 * (1+i),280,centx[i]+32,0xFFFF,0x0000);
		IERG3810_TFTLCD_ShowChar(10 * (1+i)+120,280,centy[i]+32,0xFFFF,0x0000);
		IERG3810_TFTLCD_ShowChar(10 * (1+i),260,cov[i]+32,0xFFFF,0x0000);
		IERG3810_TFTLCD_ShowChar(10 * (1+i),30,press[i]+32,0xFFFF,0x0000);
		
	}
	*/

	
	
		

	while(1)
	{
	cnt++;

	if(state == 0){
		key_start = 1;
		//For Default menu
		if(state_new == 0){
			//Put the menu function here
			//Put the menu function here
			//Put the menu function here
			//Put the menu function here
			//Put the menu function here
			//Put the menu function here
			//Put the menu function here
			state_new = 1;
		}
		if(key_select){
			gamemode = (gamemode + 1) % 2;
			if(gamemode == 1){
				/*
					for (i = 0; i < 9; i++) {
						for (j = 27; j < 36; j++) {
							if (button1_on[i][j]) {
								color = switch_color(button1_on[i][j]);
								IERG3810_Draw_DrawSquare_Menu(color, 35 + startx + j * 3, 50 +  i * 3);
							}
						}
					}
				*/
			}else{
				/*
				for (i = 0; i < 9; i++) {
					for (j = 27; j < 32; j++) { 
						if (button2_on[i][j]) {
							color = switch_color(button2_on[i][j]);
							IERG3810_Draw_DrawSquare_Menu(color, 35 + startx + j * 3, 80 +  i * 3);
						}
					}
				}
				*/
			}
		}
		if(key_start){
			state = 3;	//goto counting down
			TimerHeartBeat = 1000;
		}
		
	}else if(state == 1){
		if(game_new){
			Bag7cnt = 0;
			generate_7bag();
			random_block_generator();
			Playfield_init();
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
			}else{
				Draw_linecount();
			}
			TimerHeartBeat = 0;
		}
		if(gamemode == 1){
			//For 40 Lines
			if(line_count >= 40){
				state = 4;	//win page
			}
			
		}
		if(gamemode == 2){
			//For Blitz
			if(TimerHeartBeat >= 120000){
				state = 5;	//Blitz end page
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
		
		if(task1HeartBeat % 200 == 0){
					
					task1HeartBeat = 0;
					Draw_update();
		}
	}else if(state == 2){
		//For death page
		//Put the death page function here
		//Put the death page function here
		//Put the death page function here
		//Put the death page function here
		//Put the death page function here
		//Put the death page function here
		//Put the death page function here
		//Put the death page function here
		//Put the death page function here
	}else if(state == 3){
		//counting down
		if(TimerHeartBeat >= 4000){
			state = 1;
			IERG3810_TIM3_Init(14399 / DAS * 10,7199);
			TIM3->CNT = 0;
			continue;
		}
		if(TimerHeartBeat % 1000 == 0){
			IERG3810_TFTLCD_FillRectangle(0x0000,82,75,90,140);
			Delay(100);
			IERG3810_TFTLCD_SevenSegment(0xFFFF,82,90,4 - TimerHeartBeat/1000);
		}
	}else if(state == 4){
		//For win the 40 lines

	}else if(state == 5){
		//For time end of Blitz

	}


	//Call Joypad

	if(task2HeartBeat > 5){
		Joypad_sendpulse();
		Joypad_input_recog();
		task2HeartBeat = 0;
	}
	}
}
