#include "stm32f10x.h"
#include "Global.h"

u16 randnum;
u16 Playfield[18][28];
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
int block_pos_x_movement_tmp = 0,block_pos_y_movement_tmp = 0;
u16 cnt = 0;
u8 block_center_offset_x=0,block_center_offset_y=0;
u8 block_center_x = 1;
u8 block_center_y = 2;
u8 direction;
u8 iso;
u16 Blocks[14]={0,1,2,3,4,5,6,4,2,1,3,5,6,4};
u16 Bag7cnt = 0;
u16 PRE_block[4][4];