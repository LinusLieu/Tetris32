#include "stm32f10x.h"
#include "Global.h"
#include "IERG3810_Draw.h"

Bottom_check(void)
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

void Block_autoDrop(void)
{
    if(DAS_Timer*DAS > 100)
    {
        if(Bottom_check()){
            insert_block();
        }else{
            block_pos_y--;
            Delay(100000);
	        Draw_playfield(Playfield);
		    Delay(100000);
	        Draw_block(block);
        }
        DAS_Timer=0; 
    }
}
