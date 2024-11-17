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
		case 8:
			return 0xFFFF;
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
	for(i = 4; i < 14; i++)
	{
		for(j = 4; j < 28; j++)
		{
			color = switch_color(Playfield[i][j]);
			IERG3810_Draw_DrawSquare(color, 80+(i-4)*8, (j-4)*8+64);
		}
	}
}

void Draw_playfield_2(void)
{
	int i = 0, j = 0;
	int color = 0;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			if(block_pos_x_pre+(1-block_center_x)+i > 0 && block_pos_x_pre+(1-block_center_x)+i < 11 && block_pos_y_pre+(2-block_center_y) + j > 2 && block_pos_y_pre +(2-block_center_y)+ j < 28){
				color = switch_color(Playfield[block_pos_x_pre+(1-block_center_x)+i+3][block_pos_y_pre+(2-block_center_y)+j+1]);
				IERG3810_Draw_DrawSquare(color, 80+(block_pos_x_pre+(1-block_center_x)+i-1)*8, (block_pos_y_pre+(2-block_center_y)+j-3)*8+64);
			}
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
			//if(block_pos_x+i+1 > 0 && block_pos_x+i < 10){
			if(block_pos_x+(1-block_center_x)+i > 0 && block_pos_x+(1-block_center_x)+i < 11 && block_pos_y+(2-block_center_y) + j > 2 && block_pos_y +(2-block_center_y)+ j < 28){
			if(block[i][j]){
				color = switch_color(block[i][j]);
			}else{
				color = switch_color(Playfield[block_pos_x+(1-block_center_x)+i+3][block_pos_y+(2-block_center_y)+j+1]);
			}
			IERG3810_Draw_DrawSquare(color, 80+(block_pos_x+(1-block_center_x)+i-1)*8, (block_pos_y+(2-block_center_y)+j-3)*8+64);
			}
		}
	}
}

void Playfield_init(void){
	int i,j;

	for(i = 0;i<18;i++){
		for(j = 0;j<28;j++){
			Playfield[i][j] = 100;
		}
	}
	for(i = 4; i < 14; i++)
	{
		for(j = 4; j < 24; j++)
		{
			Playfield[i][j] = 0;
		}
		for(j = 24; j < 28; j++)
		{
			Playfield[i][j] = 12;
		}
	}

}

void Draw_update(void){
	IERG3810_DS0(1);
    Delay(50000);
	Draw_playfield_2();
	Delay(50000);
	Draw_block();
    IERG3810_DS0(0);
}