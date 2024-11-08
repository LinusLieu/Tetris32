#include "stm32f10x.h"
#include "IERG3810_USART.h"
#include "IERG3810_Clock.h"
#include "IERG3810_LED.h"
#include "Global.h"
#include "IERG3810_interrupt.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_Draw.h"
#include "Block_autoDrop.h"

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

int main(void)
{
	u8 thread = 0;
	int i = 0, j = 0;
	DAS = 1.5f;
	block_pos_x = 5;
	block_pos_y = 22;
	IERG3810_SYSTICK_Init10ms();
	IERG3810_clock_tree_init();

	block[0][0] = switch_color(1);
	block[1][0] = switch_color(1);
	block[2][0] = switch_color(1);
	block[2][1] = switch_color(1);
	
	IERG3810_TFTLCD_Init();
	for(i = 0; i < 10; i++)
	{
		for(j = 0; j < 24; j++)
		{
			Playfield[i][j] = 0XFFFF;
		}
	}
	Playfield[9][23] = switch_color(1);
	Playfield[8][22] = switch_color(2);
	Playfield[7][21] = switch_color(3);
	Playfield[6][20] = switch_color(4);
	Playfield[5][19] = switch_color(5);
	Playfield[4][18] = switch_color(6);
	Playfield[3][17] = switch_color(7);

  Delay(1000000);
	Draw_playfield(Playfield);
		Delay(1000000);
	Draw_block(block);
		
	while(1)
	{
		
		/*
		switch(thread){
			case 0:
				if (ps2count >= 11)
				{
					USART_print_int(2,opt);
					//EXTI->IMR &= ~(1<<11);
					if(opt == 0xF0){
						stat = 1;
					}else{
						IERG3810_PS2key(stat,opt);
						stat = 0;
					}
					checkbit = 0;
					ps2key = 0;
					ps2count = 0;
					temp = 0;
					opt = 0;
					EXTI->PR = 1<<11;
				}
				timeout--;		
		
				if (timeout == 0)	//Clear PS2 keyboard data when timeout
				{
					//IERG3810_PS2key_reset();
					timeout = 20000;
					checkbit = 0;
					ps2key = 0;
					ps2count = 0;
					temp = 0;
					opt = 0;
				}
				break;
			default:
				break;
		}
		*/
		Block_autoDrop();
	}
}