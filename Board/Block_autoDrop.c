#include "stm32f10x.h"
#include "Global.h"

void Block_autoDrop(void)
{
    if(DAS_Timer*DAS > 100)
    {
        autoDrop++;
        DAS_Timer=0;
    }
}