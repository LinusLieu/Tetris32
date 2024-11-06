#include "stm32f10x.h"
#include "IERG3810_USART.h"
#include "IERG3810_Clock.h"
#include "IERG3810_LED.h"
#include "Global.h"
#include "IERG3810_interrupt.h"
#include "IERG3810_TFTLCD.h"

void Delay(u32 count)
{
	u32 i;
	for (i=0;i<count;i++);
}


int main(void)
{
	u16 tmpcolor;
	u8 tmp;
	IERG3810_TFTLCD_Init();
	Delay(1000000);
	IERG3810_TFTLCD_FillRectangle(0x0000,0,240,0,320);
	Delay(1000000);

	IERG3810_TFTLCD_FillRectangle(0xF800,0,240,0,320);
	
	
	tmpcolor = IERG3810_TFTLCD_ReadColor(10,100);
	tmp = tmpcolor & 0xFF00;
	USART_print_int(2,tmp);
	tmp = tmpcolor & 0x00FF;
	USART_print_int(2,tmp);
	IERG3810_TFTLCD_ShowChinese(10,100,0xFFFF,tmpcolor);
	
	while(1)
	{
		tmpcolor = IERG3810_TFTLCD_ReadColor(10,100);
	tmp = tmpcolor & 0xFF00;
	USART_print_int(2,tmp);
	tmp = tmpcolor & 0x00FF;
	USART_print_int(2,tmp);
	}
}