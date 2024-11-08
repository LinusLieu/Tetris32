#ifndef IERG3810_Draw
#define RG3810_Draw
#include "stm32f10x.h"

void Delay(u32 count);
void IERG3810_TFTLCD_WrReg(u16 regval);
void IERG3810_TFTLCD_WrData(u16 data);
void LCD_Set9341_Parameter(void);
void IERG3810_TFTLCD_Init(void);
void IERG3810_TFTLCD_DrawDot(u16 x, u16 y, u16 color);
void IERG3810_TFTLCD_FillSquare(u16 color, u16 start_x, u16 start_y);
void Draw_playfield(u16 **playfield);
u16 switch_color(u16 type);

#endif