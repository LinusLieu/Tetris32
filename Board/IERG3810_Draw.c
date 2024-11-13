#include "stm32f10x.h"
#include "Global.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_Draw.h"
/*colors
Z:red:0XF800
L:orange:	0XFD20
O:yellow:0XFFE0
S:green:	0X400
I:cyan:	0X7FF
J:blue:0X1F
T:violet:	0X8010
*/
//draw acomplishment

u16 switch_color(u16 type)
{
	switch (type)
	{
		case 0:
			return 0X7BEF;
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
		default:
			return 0X8410;
		
		
	}
}

void IERG3810_Draw_DrawSquare(u16 color, u16 start_x, u16 start_y)
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

void Draw_playfield(void)
{
	int i = 0, j = 0;
	int color = 0;
	for(i = 1; i < 11; i++)
	{
		for(j = 1; j < 25; j++)
		{
			color = switch_color(Playfield[i][j]);
			IERG3810_Draw_DrawSquare(color, 80+i*8, j*8+64);
		}
	}
}

void Draw_block(void)
{
	int i = 0, j = 0;
	int color = 0;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			if(block_pos_x+i+1 > 0 && block_pos_x+i+1 < 11){
			color = switch_color(block[i][j]);
			if(!color){color = Playfield[block_pos_x+i+1][block_pos_y+j+1];}
			IERG3810_Draw_DrawSquare(color, 80+(block_pos_x+1+i)*8, (block_pos_y+1+j)*8+64);
			//autoDrop = 0;
			}
		}
	}
}

void Playfield_init(void){
	int i,j;
	for(i = 0; i < 12; i++)
	{
		for(j = 0; j < 21; j++)
		{
			Playfield[i][j] = 0;
		}
		for(j = 20; j < 25; j++)
		{
			Playfield[i][j] = 8;
		}
	}
	for(i = 0;i<12;i++){
		Playfield[i][0] = 10;
	}
	for(j = 0;j<21;j++){
		Playfield[0][j] = 10;
		Playfield[11][j] = 10;
	}

}