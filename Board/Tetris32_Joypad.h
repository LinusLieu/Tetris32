#ifndef __TETRIS32_JOYPAD_H
#define __TETRIS32_JOYPAD_H
#include "Global.h"
#include "stm32f10x.h"

void Joypad_init(void);
void Joypad_Latch_ExtiInit(void);
void Joypad_Clock_ExtiInit(void);
void Joypad_input_recog(void);
void Joypad_sendpulse(void);
#endif