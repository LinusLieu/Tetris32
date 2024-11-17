#ifndef __IERG3810_DRAW_H
#define __IERG3810_DRAW_H
#include "stm32f10x.h"
#include "Global.h"

u16 switch_color(u16 type);
void IERG3810_Draw_DrawSquare(u16 color, u16 start_x, u16 start_y);
void Draw_playfield(void);
void Draw_block(void);
void Playfield_init(void);
void Draw_playfield_2(void);
void Draw_update(void);
#endif