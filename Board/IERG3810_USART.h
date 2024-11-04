#ifndef __IERG3810_KEY_H
#define __IERG3810_KEY_H
#include "stm32f10x.h"

// put procedure header here
void IERG3810_USART1_init(u32 pclk1,u32 bound);
void IERG3810_USART2_init(u32 pclk1,u32 bound);
void USART_print(u8 USARTport, char *st);
void USART_print_int(u8 USARTport, u8 ipt);



#endif
