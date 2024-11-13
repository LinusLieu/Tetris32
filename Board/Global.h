#ifndef __GLOBAL_H
#define __GLOBAL_H
#include "stm32f10x.h"

extern u16 randnum;
extern u16 Playfield[12][25];
extern u16 block[4][4];
extern u8 seven_bag[7];
extern u8 seven_bag_2[7];
extern float DAS;
extern u16 DAS_Timer;
extern u16 autoDrop;
extern u8 key;
extern u8 block_pos_x,block_pos_y;
extern u8 thread;
extern int block_pos_x_movement,block_pos_y_movement;
extern u16 cnt;
#endif
