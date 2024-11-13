#include "stm32f10x.h"
#include "Global.h"
#include "Tetris32_CheckBlock.h"
#include "IERG3810_USART.h"

int Shift_check(void)
{
    int conv = 0;
    int i = 0, j = 0;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            conv += Playfield[block_pos_x+i+block_pos_x_movement+1][block_pos_y+j+block_pos_y_movement+1] * block[i][j];
        }
    }
    if(conv){
        block_pos_x_movement = 0;
        block_pos_y_movement = 0;
        thread = 1;
        return 1;
    }else{
    block_pos_x += block_pos_x_movement;
    block_pos_y += block_pos_y_movement;
    block_pos_x_movement = 0;
    block_pos_y_movement = 0;
    thread = 3;
    return 0;
    }
}

int Bottom_check_conv(void)
{
    int conv = 0;
    int i = 0, j = 0;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            conv += Playfield[block_pos_x+i+block_pos_x_movement+1][block_pos_y+j+block_pos_y_movement+1] * block[i][j];
        }
    }
    if(conv){
        block_pos_x_movement = 0;
        block_pos_y_movement = 0;
        thread = 5;
        return 1;
    }else{
    block_pos_x += block_pos_x_movement;
    block_pos_y += block_pos_y_movement;
    block_pos_x_movement = 0;
    block_pos_y_movement = 0;
    thread = 3;
    return 0;
    }
}


void insert_block(void)
{
    int i = 0,j = 0;  
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            Playfield[block_pos_x+i+1][block_pos_y+j+1] = block[i][j];
        }
    }

    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
                block[i][j] = 0;
        }
    }
    thread = 3;
}

/*
void Block_movement(u8 movement){
    int move = 0;

    int i = 0, j = 0;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            if(block[i][j] != 0){
                if(Playfield[block_pos_x-1+i][block_pos_y-1+j] != 0){
                    return 1;
                }else{
                    return 0;
                }
            }
            
        }
    }
}
*/