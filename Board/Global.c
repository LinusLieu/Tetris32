#include "stm32f10x.h"
#include "Global.h"

u16 randnum;

u16 Playfield[18][26];
u16 block[4][4];
u8 seven_bag[7];
u8 seven_bag_2[7];
u16 rotate_block[4][4];

u8 DAS,ARR;
u8 DAS_multiply,DAS_multiply_tmp;
u16 DAS_Timer = 0;
u16 ARR_Timer = 0;
u16 autoDrop = 0;
u8 shift_delay,shift_delay_cnt,shift_DAS,shift_DAS_cnt;
u8 ASP,softdrop;
u8 thread,tmpthread;

int block_pos_x,block_pos_y;     //the position of center in playfield[10][20]
u8 block_generate_pos_x,block_generate_pos_y;
int block_pos_x_pre,block_pos_y_pre;     //the previous value of position

int block_pos_x_movement = 0,block_pos_y_movement = 0;
int block_pos_x_movement_tmp = 0,block_pos_y_movement_tmp = 0;

int block_center_offset_x=0,block_center_offset_y=0;
u8 block_center_x = 1;      //position of center in block
u8 block_center_y = 2;      //position of center in block
u8 direction;

u8 iso;

u16 cnt = 0;
u16 Blocks[14]={0,1,2,3,4,5,6,4,2,1,3,5,6,4};
u16 Bag7cnt = 0;
u16 PRE_block[4][4];

u8 task1HeartBeat = 0;
u8 task2HeartBeat = 0;
u32 TimerHeartBeat = 0;

u8 key = 0;
u16 joypadkey[8];
u8 key_count;
u16 joypad_timer,joypad_timer_max;
u8 pressed[8] = {0,0,0,0,0,0,0,0};

u8 debug = 0;
u8 state = 1,gamemode = 0;
u8 key_select = 0,key_start = 0;
u8 line_count = 0;
u8 help = 0;
u32 score = 0;
u8 temp;