#include "stm32f10x.h"
#include "IERG3810_USART.h"
#include "IERG3810_Clock.h"
#include "IERG3810_LED.h"
#include "Global.h"
#include "IERG3810_interrupt.h"
#include "IERG3810_TFTLCD.h"


/*colors
Z:red£º0XF800
L:orange£º	0XFD20
O:yellow£º0XFFE0
S:green£º	0X400
I:cyan£º	0X7FF
J:blue£º0X1F
T:violet£º	0X8010
*/

void Delay(u32 count)
{
	u32 i;
	for (i=0;i<count;i++);
}

void IERG3810_TFTLCD_FillSquare(u16 color, u16 start_x, u16 start_y)
{
	u32 index = 0;
	IERG3810_TFTLCD_WrReg(0x2A);
		IERG3810_TFTLCD_WrData(start_x>>8);
		IERG3810_TFTLCD_WrData(start_x & 0xFF);
		IERG3810_TFTLCD_WrData((8 + start_x - 1) >> 8);
		IERG3810_TFTLCD_WrData((8 + start_x - 1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2B);
		IERG3810_TFTLCD_WrData(start_y>>8);
		IERG3810_TFTLCD_WrData(start_y & 0xFF);
		IERG3810_TFTLCD_WrData((8 + start_y - 1) >> 8);
		IERG3810_TFTLCD_WrData((8 + start_y - 1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2c);
	for(index=0;index<64;index++)
	{
		IERG3810_TFTLCD_WrData(color);
	}
}

void Draw_playfield(u16 playfield[10][24])
{
	int i = 0, j = 0;
	int color = 0;
	for(i = 0; i < 10; i++)
	{
		for(j = 0; j < 24; j++)
		{
			color = playfield[i][j];
			IERG3810_TFTLCD_FillSquare(color, 80+i*8, j*8+64);
		}
	}
}

void Draw_block(u16 block[4][4],u16 x,u16 y)
{
	int i = 0, j = 0;
	int color = 0;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			color = block[i][j];
			IERG3810_TFTLCD_FillSquare(color, 80+((i+x)*8), (j+y)*8+64);
		}
	}
}


u16 switch_color(u16 type)
{
	switch (type)
	{
		case 1:
			return 0XF800;
		case 2:
			return 0XFD20;
		case 3:
			return 0XFFE0;
		case 4:
			return 0X400;
		case 5:
			return 0X7FF;
		case 6:
			return 0X1F;
		case 7:
			return 0X8010;
		
		
	}
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
	Draw_block(block,4,14);
		
	while(1)
	{
	}
}