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
	block_pos_x_movement = 1;
	thread = 4;
	EXTI->PR = 1 << 4;	//Clear this exception pending bit
	cnt += 1;
}

void EXTI2_IRQHandler(void){
	block_pos_x_movement = -1;
	thread = 4;
	EXTI->PR = 1 << 2;	//Clear this exception pending bit Delay(100000);
	cnt += 3;
}

int main(void)
{
	int i = 0, j = 0;
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
	IERG3810_USART2_init(36,9600);
	
	Delay(1000000);
	IERG3810_TFTLCD_FillRectangle(0x0000,0,240,0,320);
	Delay(1000000);
	IERG3810_TFTLCD_FillRectangle(0x0000,0,240,0,320);

	block[1][0] = 3;
	block[2][0] = 3;
	block[1][1] = 3;
	block[2][1] = 3;
	
	Playfield_init();

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
				Bottom_check_conv();		//goto thread 5
				break;
			case 3:
				USART_print_int(2,0xAA);
				USART_print_int(2,block_pos_x);
				USART_print_int(2,block_pos_y);
				Delay(100000);
	      		Draw_playfield();
				Delay(100000);
	      		Draw_block();
				thread = 1;
				break;
			case 4:
				Shift_check();
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
			default:
				break;
		}
		
	}
}