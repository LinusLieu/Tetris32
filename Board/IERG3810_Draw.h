#ifndef __IERG3810_Draw
#define __IERG3810_Draw
#include "stm32f10x.h"

u16 switch_color(u16 type);
void IERG3810_Draw_DrawSquare(u16 color, u16 start_x, u16 start_y);
void Draw_playfield(u16 **playfield);

#endif