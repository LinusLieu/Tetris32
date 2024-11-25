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

//Return the Color value of Playfield and Block
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
			return 0X07E9;
		case 5:
			return 0X7FF;
		case 6:
			return 0X1F;
		case 7:
			return 0X8010;
		case 8:
			return 0xFFFF;
		default:
			return 0XF81F;
		
		
	}
}

//For dawing the elements in each block
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

//Drawing the whole playfield, for first-time setup (and old-version block update), waste a significant performance loss
void Draw_playfield(void)
{
	int i = 0, j = 0;
	int color = 0;
	
	for(i = 4; i < 14; i++)
	{
		for(j = 4; j < 24; j++)
		{
			color = switch_color(Playfield[i][j]);
			IERG3810_Draw_DrawSquare(color, 80+(i-4)*8, (j-4)*8+64);
		}
	}
}

//Drawing the previous state of the block (better performance)
void Draw_playfield_2(void)
{
	int i = 0, j = 0;
	int color = 0;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			if(block_pos_x_pre-block_center_x+i >= 0 && block_pos_x_pre-block_center_x+i < 10 && block_pos_y_pre-block_center_y + j >= 0 && block_pos_y_pre -block_center_y+ j < 26){
				color = switch_color(Playfield[block_pos_x_pre-block_center_x+i+4][block_pos_y_pre-block_center_y+j+4]);
				IERG3810_Draw_DrawSquare(color, 80+(block_pos_x_pre-block_center_x+i)*8, (block_pos_y_pre-block_center_y+j)*8+64);
			}
		}
	}
}

//Yeah...It's for drawing the block
void Draw_block(void)
{
	int i = 0, j = 0;
	int color = 0;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			//To limit the drawing range (Special design of the playfield)
			if(block_pos_x-block_center_x+i >= 0 && block_pos_x-block_center_x+i < 10 && block_pos_y-block_center_y + j >= 0 && block_pos_y -block_center_y+ j < 26){
			
			if(block[i][j]){
				color = switch_color(block[i][j]);
			}else{
				color = switch_color(Playfield[block_pos_x-block_center_x+i+4][block_pos_y-block_center_y+j+4]);
			}
			IERG3810_Draw_DrawSquare(color, 80+(block_pos_x-block_center_x+i)*8, (block_pos_y-block_center_y+j)*8+64);
			}
		}
	}
}

void Draw_Preview_2(void)
{	
	int i = 0, j = 0;
	int color = 0;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			//To limit the drawing range (Special design of the playfield)
			if(PRE_block[i][j]){
				color = switch_color(PRE_block[i][j]);
			}else{
				color = 0x0000;
			}
			IERG3810_Draw_DrawSquare(color, 172+i*8, j*8+64+100);
		}
	}
}

void Draw_Preview_init(void){
	u8 txt[4] = {78,69,88,84};
	u8 i;
	for(i = 0;i<4;i++){
		IERG3810_TFTLCD_ShowChar(10 * (i)+170,205,txt[i],0xFFFF,0x0000);
	}
}

//Actually, the playfield is 10*20, but we add 4 more blocks on the left, right and bottom for edge detect, and 4 more on the top for generate blocks
void Playfield_init(void){
	int i,j;

	for(i = 0;i<18;i++){
		for(j = 0;j<26;j++){
			Playfield[i][j] = 100;
		}
	}
	for(i = 4; i < 14; i++)
	{
		for(j = 4; j < 24; j++)
		{
			Playfield[i][j] = 0;
		}
		for(j = 24; j < 26; j++)
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

void Draw_linecount_40(void){
	u8 display_linecount[6] = {0,0,0,47,52,48};
	u8 i;
	display_linecount[0] = line_count / 100 + 48;
	display_linecount[1] = (line_count % 100) / 10 + 48;
	display_linecount[2] = line_count % 10 + 48;
	for(i = 1;i<6;i++){
		IERG3810_TFTLCD_ShowChar(10 * (i)+150,64,display_linecount[i],0xFFFF,0x0000);
	}
}

void Draw_linecount(void){
	u8 display_linecount[3];
	u8 i;
	display_linecount[0] = line_count / 100;
	display_linecount[1] = (line_count % 100) / 10;
	display_linecount[2] = line_count % 10;
	if(display_linecount[0]){
			IERG3810_TFTLCD_ShowChar(170,64,display_linecount[i],0xFFFF,0x0000);
		}
	for(i = i;i<3;i++){
		IERG3810_TFTLCD_ShowChar(10 * (i)+170,64,display_linecount[i],0xFFFF,0x0000);
	}
}

void Draw_Timer(void){
	u8 timer[6] = {0,10,0,0,10,0};
	u8 i;
	timer[0] = TimerHeartBeat / 1000 / 60;
	timer[2] = (TimerHeartBeat - timer[0]*60000) / 1000 / 10;
	timer[3] = (TimerHeartBeat - timer[0]*60000) / 1000 % 10;
	timer[5] = (TimerHeartBeat % 1000) / 100;
	for(i = 0;i<6;i++){
		IERG3810_TFTLCD_ShowChar(10 * i+90,40,timer[i] + 48,0xFFFF,0x0000);
	}
}