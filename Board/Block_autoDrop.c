#include "stm32f10x.h"
#include "Global.h"
#include "IERG3810_Draw.h"

void Block_autoDrop(void)
{
    if(DAS_Timer*DAS > 100)
    {
        block_pos_y--;
        DAS_Timer=0;
        Delay(100000);
	Draw_playfield(Playfield);
		Delay(100000);
	Draw_block(block);
    }
}