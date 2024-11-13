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

void IERG3810_SYSTICK_Init10ms(void)
{
	SysTick->CTRL = 0;	//clear
	SysTick->LOAD = 89999;	//What should be filled? Refer to DDI-0337E page 8-10
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
	thread = 7;
	EXTI->PR = 0x00000001;	//Clear this exception pending bit
}

int main(void)
{
	int i = 0, j = 0;
	u8 posx[12] = {48,79,83,63,56,0,0,0,0,0,0,0};
	u8 posy[12] = {48,79,83,63,57,0,0,0,0,0,0,0};
	u8 centx[12] = {35,69,78,84,69,82,63,56,0,0,0,0};
	u8 centy[12] = {35,69,78,84,69,82,63,57,0,0,0,0};
	u8 cov[12] = {67,79,78,86,0,0,0,0,0,0,0,0};
	u8 tmp;
	thread = 1;
	
	DAS = 1.5f;
	block_pos_x = 4;
	block_pos_y = 10;
	IERG3810_SYSTICK_Init10ms();
	IERG3810_clock_tree_init();
	IERG3810_TFTLCD_Init();
	IERG3810_NVIC_SetPriorityGroup(5);
	IERG3810_key0_ExtiInit();
	IERG3810_key2_ExtiInit();
	IERG3810_keyUP_ExtiInit();
	IERG3810_USART2_init(36,9600);
	
	direction = 0;
	
	Delay(1000000);
	IERG3810_TFTLCD_FillRectangle(0x0000,0,240,0,320);
	Delay(1000000);
	IERG3810_TFTLCD_FillRectangle(0x0000,0,240,0,320);

	block[1][0] = 3;
	block[2][0] = 3;
	block[1][1] = 3;
	block[2][1] = 3;
	
	Playfield_init();

	for(i = 0;i<12;i++)
	{
		IERG3810_TFTLCD_ShowChar(10 * (1+i),300,posx[i]+32,0xFFFF,0x0000);
		IERG3810_TFTLCD_ShowChar(10 * (1+i)+120,300,posy[i]+32,0xFFFF,0x0000);
		IERG3810_TFTLCD_ShowChar(10 * (1+i),280,centx[i]+32,0xFFFF,0x0000);
		IERG3810_TFTLCD_ShowChar(10 * (1+i)+120,280,centy[i]+32,0xFFFF,0x0000);
		IERG3810_TFTLCD_ShowChar(10 * (1+i),260,cov[i]+32,0xFFFF,0x0000);
	}

/*
	cnt = 2546;
	random_block_generator();
	cnt = 0;
*/
  Delay(1000000);
	Draw_playfield();
	Delay(1000000);
	Draw_block();
		
		

	while(1)
	{
		//USART_print_int(2,thread);
		cnt++;
		switch(thread){
			case 1:
				Block_autoDrop();
				break;
			case 2:
				tmp = Bottom_check_conv();		//goto thread 5
				cov[9] = (tmp >> 4) & 0xF;
				cov[10] = tmp & 0xF;
				for(i = 9;i<11;i++)
				{
					IERG3810_TFTLCD_ShowChar(10 * (1+i),260,cov[i]+48,0xFFFF,0x0000);
				}
				break;
			case 3:
				USART_print_int(2,0xAA);
				USART_print_int(2,block_pos_x);
				USART_print_int(2,block_pos_y);
				posx[9] = (block_pos_x >> 4) & 0xF;
				posx[10] = block_pos_x & 0xF;
				posy[9] = (block_pos_y >> 4) & 0xF;
				posy[10] = block_pos_y & 0xF;
				for(i = 9;i<11;i++)
				{
					IERG3810_TFTLCD_ShowChar(10 * (1+i),300,posx[i]+48,0xFFFF,0x0000);
					IERG3810_TFTLCD_ShowChar(10 * (1+i)+120,300,posy[i]+48,0xFFFF,0x0000);
				}
				Delay(100000);
	      		Draw_playfield();
				Delay(100000);
	      		Draw_block();
				thread = 1;
				break;
			case 4:
				tmp = Shift_check();
				cov[9] = (tmp >> 4) & 0xF;
				cov[10] = tmp & 0xF;
				for(i = 9;i<11;i++)
				{
					IERG3810_TFTLCD_ShowChar(10 * (1+i),260,cov[i]+48,0xFFFF,0x0000);
				}
				break;
			case 5:
				insert_block();
				thread = 6;
				break;
			case 6:
				USART_print_int(2,0xAF);
				random_block_generator();
				block_pos_x = 4;
				block_pos_y = 10;
				thread = 1;
				break;
			case 7:
				rotate_clockwise();
				thread = 4;
			default:
				break;
		}
		
	}
}