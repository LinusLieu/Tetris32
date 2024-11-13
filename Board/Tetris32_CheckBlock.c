#include "stm32f10x.h"
#include "Global.h"
#include "Tetris32_CheckBlock.h"

int Bottom_check(void)
{
    
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

void insert_block(void)
{
    int i = 0,j = 0;  
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            Playfield[block_pos_x+i][block_pos_y+j] = block[i][j];
        }
    }

}