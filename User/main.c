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

int main(void)
{
	
	int i = 0, j = 0;


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

  Delay(12000000);
	Draw_playfield(Playfield);
	Draw_block(block);
		
	while(1)
	{
		Block_autoDrop();
	}
}