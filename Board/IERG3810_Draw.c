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
			if(block_pos_x_pre-block_center_x+i >= 0 && block_pos_x_pre-block_center_x+i < 10 && block_pos_y_pre-block_center_y + j >= 0 && block_pos_y_pre -block_center_y+ j < 20){
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
			if(block_pos_x-block_center_x+i >= 0 && block_pos_x-block_center_x+i < 10 && block_pos_y-block_center_y + j >= 0 && block_pos_y -block_center_y+ j < 20){
			
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
	u8 display_linecount[6] = {0,0,0,47,52,48};
	u8 i;
	display_linecount[0] = line_count / 100 + 48;
	display_linecount[1] = (line_count % 100) / 10 + 48;
	display_linecount[2] = line_count % 10 + 48;
	for(i = 0;i<3;i++){
		IERG3810_TFTLCD_ShowChar(10 * (i)+160,64,display_linecount[i],0xFFFF,0x0000);
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

void Draw_Timer_2(void){
	u8 timer[6] = {0,10,0,0,10,0};
	u8 i;
	timer[0] = score / 1000 / 60;
	timer[2] = (score - timer[0]*60000) / 1000 / 10;
	timer[3] = (score - timer[0]*60000) / 1000 % 10;
	timer[5] = (score % 1000) / 100;
	for(i = 0;i<6;i++){
		IERG3810_TFTLCD_ShowChar(10 * i+90,140,timer[i] + 48,0xFFFF,0x0000);
	}
}

void IERG3810_Draw_DrawSquare_Menu(u16 color, u16 start_x, u16 start_y)
{
	u32 index = 0;
	IERG3810_TFTLCD_WrReg(0x2A);
		IERG3810_TFTLCD_WrData(start_x>>8);
		IERG3810_TFTLCD_WrData(start_x & 0xFF);
		IERG3810_TFTLCD_WrData((5 + start_x - 1) >> 8);
		IERG3810_TFTLCD_WrData((5 + start_x - 1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2B);
		IERG3810_TFTLCD_WrData(start_y>>8);
		IERG3810_TFTLCD_WrData(start_y & 0xFF);
		IERG3810_TFTLCD_WrData((5 + start_y - 1) >> 8);
		IERG3810_TFTLCD_WrData((5 + start_y - 1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2c);
	
	for(index=0;index<25;index++)
	{
		IERG3810_TFTLCD_WrData(color);
	}
}

void Draw_Tetris_Letters(void)
{
    int i, j, n;
    u16 startx = 15;  // 字母起始X坐标
    u16 starty = 200; // 字母起始Y坐标
    u16 color = 0;    // 方块颜色
	u16 letters_with_gap[5][35] = {
    // 第一行：旋转后的字母 T, E, T, R, I, S 之间插入间隔
    {0,0,1,0,0,  0,  2,2,2,2,2,  0,  0,0,3,0,0,  0,  4,0,0,0,4,  0,  0,5,5,5,0,  0,  6,6,6,6,6},

    // 第二行：旋转后的字母 T, E, T, R, I, S 之间插入间隔
    {0,0,1,0,0,  0,  2,0,0,0,0,  0,  0,0,3,0,0,  0,  4,0,0,4,0,  0,  0,0,5,0,0,  0,  0,0,0,0,6},

    // 第三行：旋转后的字母 T, E, T, R, I, S 之间插入间隔
    {0,0,1,0,0,  0,  2,2,2,0,0,  0,  0,0,3,0,0,  0,  4,4,4,0,0,  0,  0,0,5,0,0,  0,  0,6,6,6,6},

    // 第四行：旋转后的字母 T, E, T, R, I, S 之间插入间隔
    {0,0,1,0,0,  0,  2,0,0,0,0,  0,  0,0,3,0,0,  0,  4,0,0,4,0,  0,  0,0,5,0,0,  0,  0,6,0,0,0},

    // 第五行：旋转后的字母 T, E, T, R, I, S 之间插入间隔
    {1,1,1,1,1,  0,  2,2,2,2,0,  0,  3,3,3,3,3,  0,  4,4,4,0,0,  0,  0,5,5,5,0,  0,  0,6,6,6,6}
	};
	u16 letters_2[5][11] = {
	{7,7,7,7,7,0,1,1,1,1,1},
	{0,0,0,0,7,0,1,0,0,0,0},
	{0,7,7,7,7,0,1,1,1,1,1},
	{0,0,0,0,7,0,0,0,0,0,1},
	{7,7,7,7,7,0,1,1,1,1,1},
	};
	u16 letters_5[15][20] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0},
    {0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0},
    {0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};
	u16 button1_N[9][26] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 3, 3, 0, 0, 5, 5, 5, 5, 0, 3, 3, 3, 3, 0, 0, 7, 7, 0, 0, 2, 2, 2, 2},
    {0, 0, 3, 0, 0, 3, 0, 5, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 7, 7, 0, 0, 2, 2, 0, 0},
    {0, 0, 3, 3, 3, 0, 0, 5, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 7, 7, 0, 0, 0, 2, 2, 0},
    {0, 0, 3, 0, 0, 3, 0, 5, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 7, 7, 0, 0, 0, 0, 2, 2},
    {0, 0, 3, 3, 3, 0, 0, 5, 0, 0, 0, 0, 3, 3, 3, 3, 0, 7, 7, 7, 7, 0, 2, 2, 2, 2},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};
	u16 button2_N[9][31] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 3, 0, 0, 0, 1, 1, 0, 0, 2, 2, 2, 2, 0, 5, 5, 5, 0, 6, 0, 0, 6, 0, 7, 7, 7, 7, 0},
    {0, 0, 3, 3, 3, 3, 0, 1, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0, 5, 0, 0, 6, 0, 6, 6, 0, 7, 0, 0, 0, 0},
    {0, 0, 3, 0, 3, 0, 0, 1, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0, 5, 0, 0, 6, 6, 0, 6, 0, 7, 7, 7, 7, 0},
    {0, 0, 0, 3, 3, 0, 0, 1, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0, 5, 0, 0, 6, 0, 0, 6, 0, 7, 0, 0, 0, 0},
    {0, 0, 0, 0, 3, 0, 0, 0, 1, 1, 0, 0, 2, 0, 0, 0, 0, 5, 5, 5, 0, 6, 0, 0, 6, 0, 7, 7, 7, 7, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};
	u8 button3[9][21] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,5,0,0,5,0,5,5,5,5,0,5,5,5,5,0,5,0,0,0},
		{0,0,5,0,0,5,0,5,0,0,0,0,5,0,0,0,0,5,0,0,0},
		{0,0,5,5,5,5,0,5,5,5,5,0,5,0,0,0,0,5,5,5,0},
		{0,0,5,0,0,5,0,5,0,0,0,0,5,0,0,0,0,5,0,0,5},
		{0,0,5,0,0,5,0,5,5,5,5,0,5,0,0,0,0,5,5,5,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};
	u8 button4[9][26] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,6,0,0,6,0,6,6,6,0,0,0,6,6,0,0,0,6,6,0,0,0,6,6,0},
		{0,0,6,0,0,6,0,6,0,0,6,0,6,0,0,6,0,6,0,0,6,0,0,6,6,0},
		{0,0,6,6,6,6,0,6,6,6,0,0,6,0,0,6,0,6,0,0,6,0,0,6,6,0},
		{0,0,6,0,0,6,0,6,0,0,6,0,6,0,0,6,0,6,0,0,6,0,6,6,6,6},
		{0,0,0,6,6,0,0,6,6,6,0,0,0,6,6,0,0,6,0,0,6,0,6,6,6,6},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	};
	u8 help_char[23] = {80,114,101,115,115,32,75,101,121,95,85,80,32,102,111,114,32,104,101,108,112,46};

	IERG3810_TFTLCD_FillRectangle(0x137A,25,195,225,60);
	IERG3810_TFTLCD_FillRectangle(0x0A0F,30,185,230,50);
	IERG3810_TFTLCD_FillRectangle(0x137A,90,65,170,60);
	IERG3810_TFTLCD_FillRectangle(0x0A0F,95,55,175,60);

    // 遍历每个字母
    for (i = 0; i < 5; i++) {  // 5行
        for (j = 0; j < 35; j++) {  // 35列
            if (letters_with_gap[i][j]) { // 如果该位置有方块
                color = switch_color(letters_with_gap[i][j]);  // 获取颜色
                // 绘制该位置的方块
                IERG3810_Draw_DrawSquare_Menu(color, 20+startx + j * 5, starty + 40 + i * 5);
            }
        }
    }
	for (i = 0; i < 5; i++) {  // 5行
        for (j = 0; j < 11; j++) {  // 35列
            if (letters_2[i][j]) { // 如果该位置有方块
                color = switch_color(letters_2[i][j]);  // 获取颜色
                // 绘制该位置的方块
                IERG3810_Draw_DrawSquare_Menu(color, 20+startx + (12 + j) * 5, starty - 10 +  i * 5);
            }
        }
    }
	for (i = 0; i < 15; i++) {  // 5行
        for (j = 0; j < 20; j++) {  // 35列
            if (letters_5[i][j]) { // 如果该位置有方块
                color = switch_color(letters_5[i][j]);  // 获取颜色
                // 绘制该位置的方块
                IERG3810_Draw_DrawSquare_Menu(color, 150 + startx + j * 3, starty + -180 +  i * 3);
            }
        }
    }
		/*for (i = 0; i < 8; i++) {  // 5行
        for (j = 0; j < 8; j++) {  // 35列
            if (letters_L[i][j]) { // 如果该位置有方块
                color = switch_color(letters_L[i][j]);  // 获取颜色
                // 绘制该位置的方块
                IERG3810_Draw_DrawSquare_Menu(color, startx + j * 3, starty + 80 +  i * 3);
            }
        }
    }*/
	for (i = 0; i < 9; i++) {  // 5行
        for (j = 0; j < 26; j++) {  // 35列
            if (button1_N[i][j]) { // 如果该位置有方块
                color = switch_color(button1_N[i][j]);  // 获取颜色
                // 绘制该位置的方块
                IERG3810_Draw_DrawSquare_Menu(color, 60 + startx + j * 3, 110 +  i * 3);
            }
        }
    }
		
	for (i = 0; i < 9; i++) {  // 5行
        for (j = 0; j < 31; j++) {  // 35列
            if (button2_N[i][j]) { // 如果该位置有方块
                color = switch_color(button2_N[i][j]);  // 获取颜色
                // 绘制该位置的方块
                IERG3810_Draw_DrawSquare_Menu(color, 60 + startx + j * 3, 80 +  i * 3);
            }
        }
    }

	for (i = 0; i < 9; i++) {  // 5行
        for (j = 0; j < 21; j++) {  // 35列
            if (button3[i][j]) { // 如果该位置有方块
                color = switch_color(button3[i][j]);  // 获取颜色
                // 绘制该位置的方块
                IERG3810_Draw_DrawSquare_Menu(color, 60 + startx + j * 3, 50 +  i * 3);
            }
        }
    }

	for (i = 0; i < 9; i++) {  // 5行
        for (j = 0; j < 26; j++) {  // 35列
            if (button4[i][j]) { // 如果该位置有方块
                color = switch_color(button4[i][j]);  // 获取颜色
                // 绘制该位置的方块
                IERG3810_Draw_DrawSquare_Menu(color, 60 + startx + j * 3, 20 +  i * 3);
            }
        }
    }
}

void Draw_Menu_Arror(u8 light,u8 mode)
{	
	IERG3810_TFTLCD_FillRectangle(0x0000,60,10,30,110);
	if(light){
		IERG3810_TFTLCD_FillRectangle(0xFFFF,60,10,120 - ((mode) * 30),10);
	}
	
	
}

void Draw_help_page(void){
	u8 i,j;
	u8 help_char[13][20] = {
	{66, 108, 105, 116, 122, 58,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {84, 119, 111, 45, 109, 105, 110, 117, 116, 101, 32, 99, 111, 117, 110, 116, 100, 111, 119, 110},
    {52, 48, 76, 105, 110, 101, 58,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {69, 108, 105, 109, 105, 110, 97, 116, 101, 32, 52, 48, 32, 108, 105, 110, 101, 115,0,0},
    {76, 101, 102, 116, 47, 82, 105, 103, 104, 116,0,0,0,0,0,0,0,0,0,0},
    {102, 111, 114, 32, 109, 111, 118, 105, 110, 103, 32, 116, 104, 101, 32, 98, 108, 111, 99, 107},
	{68, 111, 119, 110, 58,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {116, 111, 32, 115, 112, 101, 101, 100, 32, 117, 112,0,0,0,0,0,0,0,0,0},
	{116, 104, 101, 32, 102, 97, 108, 108, 105, 110, 103,0,0,0,0,0,0,0,0,0},
    {65, 58, 32, 82, 111, 116, 97, 116, 101, 32, 99, 108, 111, 99, 107, 119, 105, 115, 101,0},
    {66, 58, 32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{82, 111, 116, 97, 116, 101,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{97, 110, 116, 105, 45, 99, 108, 111, 99, 107, 119, 105, 115, 101,0,0,0,0,0,0},
	};

	for(i = 0;i<20;i++){
		for(j = 0;j<13;j++){
			IERG3810_TFTLCD_ShowChar(10 * (1+i),300 - 18 * j,help_char[j][i],0xFFFF,0x0000);
		}
	}
}

void Draw_name_page(void){
	u8 i,j;
	u8 name_char[5][20] = {
	{77, 97, 100, 101, 32, 66, 121, 32, 87, 48, 55, 58,0,0,0,0,0,0,0,0},
	{76, 73, 69, 85, 32, 87, 97, 110, 103, 32, 75, 105, 116,0,0,0,0,0,0,0},
	{49, 49, 53, 53, 49, 57, 50, 48, 52, 48,0,0,0,0,0,0,0,0,0,0},
	{90, 72, 65, 78, 71, 32, 82, 117, 105, 32, 74, 105, 110, 103,0,0,0,0,0,0},
	{49, 49, 53, 53, 49, 57, 49, 52, 52, 51,0,0,0,0,0,0,0,0,0,0}
	};
	
	for(i = 0;i<20;i++){
		for(j = 0;j<5;j++){
			IERG3810_TFTLCD_ShowChar(10 * (1+i) + 50,250 - 18 * j,name_char[j][i],0xFFFF,0x0000);
		}
	}
}

void Draw_end_40line(void){
	u8 i,j;
	u8 cong[16] = {67, 111, 110, 103, 114, 97, 116, 117, 108, 97, 116, 105, 111, 110, 115, 33};
	u8 sent2[14] = {89, 111, 117, 114, 32, 115, 99, 111, 114, 101, 32, 105, 115, 58};

	IERG3810_TFTLCD_FillRectangle(0x0000,0,240,0,320);
	Delay(1000000);
	for(i = 0;i<16;i++){
			IERG3810_TFTLCD_ShowChar(10 * i + 50,180,cong[i],0xFFFF,0x0000);
	}
	for(i = 0;i<14;i++){
			IERG3810_TFTLCD_ShowChar(10 * i + 60,160,sent2[i],0xFFFF,0x0000);
	}
	Draw_Timer_2();
}

void Draw_end_Blitz(void){
	u8 i,j;
	u8 sent1[11] = {84, 105, 109, 101, 32, 105, 115, 32, 117, 112, 33};
	u8 sent2[14] = {89, 111, 117, 32, 101, 108, 105, 109, 105, 110, 97, 116, 101, 100};
	u8 sent3[6] = {108, 105, 110, 101, 115, 46};
	u8 sent4[4] = {0,0,0,0};
	sent4[0] = score / 1000;
	sent4[1] = score / 100 % 10;
	sent4[2] = score / 10 % 10;
	sent4[3] = score % 10;
	IERG3810_TFTLCD_FillRectangle(0x0000,0,240,0,320);
	Delay(1000000);
	for(i = 0;i<11;i++){
			IERG3810_TFTLCD_ShowChar(10 * i + 65,180,sent1[i],0xFFFF,0x0000);
	}
	for(i = 0;i<14;i++){
			IERG3810_TFTLCD_ShowChar(10 * i + 60,140,sent2[i],0xFFFF,0x0000);
	}
	for(i = 2;i<4;i++){
			IERG3810_TFTLCD_ShowChar(10 * i + 60,120,sent4[i]+48,0xFFFF,0x0000);
	}
	for(i = 0;i<6;i++){
			IERG3810_TFTLCD_ShowChar(10 * i + 60,100,sent3[i],0xFFFF,0x0000);
	}


}

void Draw_deathpage(void){
	u8 i,j;
	u8 sent1[9] = {89, 111, 117, 32, 108, 111, 115, 101, 33};
	for(i = 0;i<10;i++){
		IERG3810_TFTLCD_FillRectangle(0xF800,0,240,(i*16),16);
		Delay(500000);
		IERG3810_TFTLCD_FillRectangle(0xF800,0,240,320 - (i*16) - 16,16);
		Delay(500000);
	}

	for(i = 0;i<9;i++){
			IERG3810_TFTLCD_ShowChar(10 * i + 75,155,sent1[i],0xFFFF,0xF800);
	}
}

void Draw_40line(void){
	u8 i,j;
	u16 color = 0;
	u16 button2_N[9][31] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 3, 0, 0, 0, 1, 1, 0, 0, 2, 2, 2, 2, 0, 5, 5, 5, 0, 6, 0, 0, 6, 0, 7, 7, 7, 7, 0},
    {0, 0, 3, 3, 3, 3, 0, 1, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0, 5, 0, 0, 6, 0, 6, 6, 0, 7, 0, 0, 0, 0},
    {0, 0, 3, 0, 3, 0, 0, 1, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0, 5, 0, 0, 6, 6, 0, 6, 0, 7, 7, 7, 7, 0},
    {0, 0, 0, 3, 3, 0, 0, 1, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0, 5, 0, 0, 6, 0, 0, 6, 0, 7, 0, 0, 0, 0},
    {0, 0, 0, 0, 3, 0, 0, 0, 1, 1, 0, 0, 2, 0, 0, 0, 0, 5, 5, 5, 0, 6, 0, 0, 6, 0, 7, 7, 7, 7, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};

	for (i = 0; i < 9; i++) {  // 5行
        for (j = 0; j < 31; j++) {  // 35列
            if (button2_N[i][j]) { // 如果该位置有方块
                color = switch_color(button2_N[i][j]);
                // 绘制该位置的方块
            }else{
				color = 0x0000;
				}
			IERG3810_TFTLCD_FillRectangle(color,(7 * j)+10,7,240 + (7 * i),7);
        }
    }
}

void Draw_blitz(void){
	u8 i,j;
	u16 color = 0;
	u16 button1_N[9][31] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 3, 3, 0, 0, 5, 5, 5, 5, 0, 3, 3, 3, 3, 0, 0, 7, 7, 0, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 0, 3, 0, 5, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 7, 7, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 3, 3, 0, 0, 5, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 7, 7, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0, 0},
    {0, 0, 3, 0, 0, 3, 0, 5, 0, 0, 0, 0, 0, 3, 3, 0, 0, 0, 7, 7, 0, 0, 0, 0, 2, 2, 0, 0, 0, 0, 0},
    {0, 0, 3, 3, 3, 0, 0, 5, 0, 0, 0, 0, 3, 3, 3, 3, 0, 7, 7, 7, 7, 0, 2, 2, 2, 2, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
	};

	for (i = 0; i < 9; i++) {  // 5行
        for (j = 0; j < 31; j++) {  // 35列
            if (button1_N[i][j]) { // 如果该位置有方块
                color = switch_color(button1_N[i][j]);
                // 绘制该位置的方块
            }else{
				color = 0x0000;
				}
			IERG3810_TFTLCD_FillRectangle(color,(7 * j)+17,7,240 + (7 * i),7);
        }
    }
}