#ifndef __TETRIS32_GENERATE_7BAG_H
#define __TETRIS32_GENERATE_7BAG_H
#include "stm32f10x.h"
#include "Global.h"
#include "Tetris32_random_block_generator.h"

u16 generate_random(void);
void generate_7bag(u16 blocks[7]);
void turn_blocks_number(u16 blocks[7]);

#endif