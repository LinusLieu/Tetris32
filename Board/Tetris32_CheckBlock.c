#include "stm32f10x.h"
#include "Global.h"
#include "Tetris32_CheckBlock.h"
#include "IERG3810_USART.h"

int Shift_check(void)
{   
    int conv = 0;
    int i = 0, j = 0;
    block_pos_x_movement = block_pos_x_movement_tmp;
    block_pos_x_movement_tmp = 0;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            conv += Playfield[block_pos_x-block_center_x+i+block_pos_x_movement+4][block_pos_y-block_center_y+j+4] * block[i][j];
        }
    }
    if(conv){
        block_pos_x_movement = 0;
        return conv;
    }else{
    block_pos_x_pre = block_pos_x;
    block_pos_y_pre = block_pos_y;
    block_pos_x += block_pos_x_movement;
    block_pos_x_movement = 0;
    return conv;
    }
}

int Bottom_check_conv(void)
{
    int conv = 0;
    int i = 0, j = 0;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            conv += Playfield[block_pos_x-block_center_x+i+4][block_pos_y-block_center_y+j+block_pos_y_movement+4] * block[i][j];
        }
    }
    if(conv){
        if(block_pos_y >= 19){
            state = 2;
            return conv;
        }
        block_pos_y_movement = 0;
        insert_block();
        random_block_generator();
		block_pos_x = block_generate_pos_x;
		block_pos_y = block_generate_pos_y;
        block_center_offset_x = 0;
        block_center_offset_y = 0;
        block_center_x = 1;
        block_center_y = 2;
        direction = 0;
        return conv;
    }else{
    block_pos_x_pre = block_pos_x;
    block_pos_y_pre = block_pos_y;
    block_pos_y += block_pos_y_movement;
    block_pos_y_movement = 0;
    thread = 3;
    return conv;
    }
}

int conv_check(void)
{   
    int conv = 0;
    int i = 0, j = 0;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            conv += Playfield[block_pos_x-block_center_x+i+4][block_pos_y-block_center_y+j+4] * block[i][j];
        }
    }
    return conv;
}

int Rotate_check(void)
{   
    int conv = 0;
    int i = 0, j = 0;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            conv += Playfield[block_pos_x-block_center_x+block_center_offset_x+i+4][block_pos_y-block_center_y+block_center_offset_y+j+4] * block[i][j];
        }
    }
    return conv;
}

void insert_block(void)
{
    int i = 0,j = 0;  
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            if(block[i][j])
            Playfield[block_pos_x-block_center_x+i+4][block_pos_y-block_center_y+j+4] = block[i][j];
        }
    }

    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
                block[i][j] = 0;
        }
    }
    iso = 0;
    remove();
}
