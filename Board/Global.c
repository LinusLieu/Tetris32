#include "stm32f10x.h"
#include "Global.h"


u16 Playfield[10][24];
u16 block[4][4];
u16 DAS = 1;
u16 DAS_Timer = 0;
u16 autoDrop = 0;