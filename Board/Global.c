#include "stm32f10x.h"
#include "Global.h"

u16 randnum;
u16 Playfield[12][25];
u16 block[4][4];
u8 seven_bag[7];
u8 seven_bag_2[7];
float DAS;
u16 DAS_Timer = 0;
u16 autoDrop = 0;
u8 key = 0;
u8 block_pos_x,block_pos_y;
u8 thread;
int block_pos_x_movement = 0,block_pos_y_movement = 0;
u16 cnt = 0;
