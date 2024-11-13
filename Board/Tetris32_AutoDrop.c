#include "stm32f10x.h"
#include "Global.h"
#include "IERG3810_Draw.h"
#include "Tetris32_AutoDrop.h"
#include "Tetris32_CheckBlock.h"

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
