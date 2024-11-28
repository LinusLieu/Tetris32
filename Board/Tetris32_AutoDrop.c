#include "stm32f10x.h"
#include "Global.h"
#include "IERG3810_Draw.h"
#include "Tetris32_AutoDrop.h"
#include "Tetris32_CheckBlock.h"
#include "IERG3810_USART.h"

void Block_autoDrop(void)
{
    if(DAS_Timer*DAS > 100)
    {
        block_pos_y_movement = -1;
        thread = 2;
        DAS_Timer=0; 
        
    }
}
