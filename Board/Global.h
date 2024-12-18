#ifndef __GLOBAL_H
#define __GLOBAL_H
#include "stm32f10x.h"

extern u16 randnum;
extern u16 Playfield[18][26];
extern u16 block[4][4];
extern u16 rotate_block[4][4];
extern u8 seven_bag[7];
extern u8 seven_bag_2[7];
extern u8 DAS,ARR;
extern u16 DAS_Timer,ARR_Timer;
extern u8 ASP,softdrop;
extern u16 autoDrop;
extern u8 shift_delay,shift_delay_cnt,shift_DAS,shift_DAS_cnt;
extern u8 key;
extern int block_pos_x,block_pos_y;
extern u8 block_generate_pos_x,block_generate_pos_y;
extern int block_pos_x_pre,block_pos_y_pre;
extern u8 thread,tmpthread;
extern int block_pos_x_movement,block_pos_y_movement;
extern int block_pos_x_movement_tmp,block_pos_y_movement_tmp;
extern u16 cnt;
extern int block_center_offset_x,block_center_offset_y;  //used for rotation, which will move the center point
extern u8 block_center_x,block_center_y;
extern u8 direction;
extern u8 iso;
extern u16 joypadkey[8];
extern u8 key_count;
extern u16 joypad_timer,joypad_timer_max;
extern u16 Blocks[14];
extern u16 Bag7cnt;
extern u16 PRE_block[4][4];
extern u8 task1HeartBeat,task2HeartBeat;
extern u32 TimerHeartBeat;
extern u8 pressed[8];
extern u8 debug;
extern u8 state,gamemode;
extern u8 key_select,key_start;
extern u8 line_count;
extern u8 help;
extern u32 score;
extern u8 temp;
#endif
