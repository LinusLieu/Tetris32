#include "stm32f10x.h"
#include "IERG3810_Clock.h"

// put your procedure and code here
void IERG3810_clock_tree_init(void)
{
	u8 PLL=7;
	unsigned char temp=0;
	RCC->CFGR &= 0xF8FF0000;	//26:24 MCO No clock, 15:14 ADC prescaler PCLK2 divided by 2, 13:11 APB2 HCLK not divided, 10:8 APB1 HCLK not divided, 7:4 AHB prescaler not divided, 3:2 HSI oscillator used as system clock, 1:0 HSI selected as system clock
	RCC->CR &= 0xFEF6FFFF;		//24 PLL OFF, 19 Clock detector OFF, 16 HSE oscillator ON
	RCC->CR |= 0x00010000;		//HESON=1, (RM0008 page-99) internal 8 MHz RC oscillator ON
	while(!(RCC->CR>>17));		//Check HSERDY, (RM0008 page-99)
	RCC->CFGR = 0x00000400;		//PPRE1=100, (RM0008 page-101) APB1 HCLK divided by 2, APB2 HCLK not divided
	RCC->CFGR |= PLL << 18;		//PLLMUL=111, (RM0008 page-101) PLL input clock x 9
	RCC->CFGR |= 1 << 16;			//PLLSRC=1, (RM0008 page-101) HSE oscillator clock selected as PLL input clock
	FLASH->ACR |= 0x32;				//set FLASH with 2 wait states 
	RCC->CR |= 0x01000000;		//PLLON=1, (RM0008 page-99) PLL enable, PLL clock is used as system clock or is selected to become the system clock
	while(!(RCC->CR>>25));		//check PLLRDY, (RM0008 page-99) PLL clock ready flag
	RCC->CFGR |= 0x00000002;	//SW=10, (RM0008 page-101) PLL selected as system clock
	while(temp != 0x02)				//check SWS (RM0008 page-103) check if PLL used as system clock
	{
			temp = RCC->CFGR>>2;
			temp &= 0x03;
	}
}
