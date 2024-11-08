#include "stm32f10x.h"
#include "Global.h"
#include "Bottom_check.h"
void Bottom_check(void)
{
    
    int i = 0, j = 0;
    for(i = 0; i < 4; i++){
        for(j = 0; j < 4; j++){
            if(block[i][j] != 0){
                if(Playfield[block_pos_x-1+i][block_pos_y-1+j] != 0){
                    insert_block;
                    break;
                }
            }
            
        }
    }
}

void insert_block(void)
{
    int i = 0, j = 0;  
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            Playfield[block_pos_x+i][block_pos_y+j] = block[i][j];
        }
    }

}