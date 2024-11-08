#include "stm32f10x.h"
#include "IERG3810_USART.h"
#include "IERG3810_Clock.h"
#include "IERG3810_LED.h"
#include "Global.h"
#include "IERG3810_interrupt.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_Draw.h"
/*colors
Z:red£º0XF800
L:orange£º	0XFD20
O:yellow£º0XFFE0
S:green£º	0X400
I:cyan£º	0X7FF
J:blue£º0X1F
T:violet£º	0X8010
*/
//draw acomplishment
typedef struct
{
	u16 LCD_REG;
	u16 LCD_RAM;
} LCD_TypeDef;

#define LCD_BASE ((u32)(0x6C000000 | 0x000007FE))
#define LCD ((LCD_TypeDef *) LCD_BASE)

void Delay(u32 count)
{
	u32 i;
	for (i=0;i<count;i++);
}

void IERG3810_TFTLCD_WrReg(u16 regval)
{
	LCD->LCD_REG=regval;
}

void IERG3810_TFTLCD_WrData(u16 data)
{
	LCD->LCD_RAM=data;
}

void LCD_Set9341_Parameter(void)
{
	IERG3810_TFTLCD_WrReg(0X01);		//Software reset
	IERG3810_TFTLCD_WrReg(0X11);		//Exit_sleep_mode
	
	IERG3810_TFTLCD_WrReg(0X3A);		//Set_pixel_format
	IERG3810_TFTLCD_WrData(0X55);	//65536 colors
	
	IERG3810_TFTLCD_WrReg(0X29);		//Display ON
	
	IERG3810_TFTLCD_WrReg(0X36);		//Memory Access Control
	IERG3810_TFTLCD_WrData(0XC8);	//control Display direction
}

void IERG3810_TFTLCD_Init(void)
{
	RCC->AHBENR |= 1<<8;		//FSMC
	RCC->APB2ENR |= 1<<3;		//PORTB
	RCC->APB2ENR |= 1<<5;		//PORTD
	RCC->APB2ENR |= 1<<6;		//PORTE
	RCC->APB2ENR |= 1<<8;		//PORTG
	GPIOB->CRL &= 0xFFFFFFF0;		//PB0
	GPIOB->CRL |= 0x00000003;
	
	//PORTD
	GPIOD->CRH &= 0x00FFF000;
	GPIOD->CRH |= 0xBB000BBB;
	GPIOD->CRL &= 0xFF00FF00;
	GPIOD->CRL |= 0x00BB00BB;
	
	//PORTE
	GPIOE->CRH &= 0x00000000;
	GPIOE->CRH |= 0xBBBBBBBB;
	GPIOE->CRL &= 0x0FFFFFFF;
	GPIOE->CRL |= 0xB0000000;
	
	//PORTg12
	GPIOG->CRH &= 0xFFF0FFFF;
	GPIOG->CRH |= 0x000B0000;
	GPIOG->CRL &= 0xFFFFFFF0;		//PG0->RS
	GPIOG->CRL |= 0x0000000B;
	
	
	// LCD uses FSMC Bank 4 memory bank.
	// Use Mode A
	FSMC_Bank1->BTCR[6] = 0x00000000;		//FSMC_BCR4 (reset)
	FSMC_Bank1->BTCR[7] = 0x00000000;		//FSMC_BTR4 (reset)
	FSMC_Bank1E->BWTR[6] = 0x00000000;	//FSMC_BWTR4 (reset)
	FSMC_Bank1->BTCR[6] |= 1<<12;		//FSMC_BCR4 -> WREN
	FSMC_Bank1->BTCR[6] |= 1<<14;		//FSMC_BCR4 -> EXTMOD
	FSMC_Bank1->BTCR[6] |= 1<<4;		//FSMC_BCR4 -> MWID
	FSMC_Bank1->BTCR[7] |= 0<<28;		//FSMC_BTR4 -> ACCMOD
	FSMC_Bank1->BTCR[7] |= 1<<0;		//FSMC_BTR4 -> ADDSET
	FSMC_Bank1->BTCR[7] |= 0xF<<8;	//FSMC_BTR4 -> DATAST
	FSMC_Bank1E->BWTR[6] |= 0<<28;	//FSMC_BWTR4 -> ACCMOD
	FSMC_Bank1E->BWTR[6] |= 0<<0;		//FSMC_BWTR4 -> ADDSET
	FSMC_Bank1E->BWTR[6] |= 3<<8;		//FSMC_BWTR4 -> DATAST
	FSMC_Bank1->BTCR[6] |= 1<<0;		//FSMC_BCR4 -> FACCEN
	LCD_Set9341_Parameter();	//special setting for LCD module
	
	GPIOB->BRR = 0x00000001;
	GPIOB->BSRR = 0x00000001;
}

void IERG3810_TFTLCD_DrawDot(u16 x, u16 y, u16 color)
{
	IERG3810_TFTLCD_WrReg(0x2A);
		IERG3810_TFTLCD_WrData(x>>8);
		IERG3810_TFTLCD_WrData(x & 0xFF);
		IERG3810_TFTLCD_WrData(0x01);
		IERG3810_TFTLCD_WrData(0x3F);
	IERG3810_TFTLCD_WrReg(0x2B);
		IERG3810_TFTLCD_WrData(y>>8);
		IERG3810_TFTLCD_WrData(y & 0xFF);
		IERG3810_TFTLCD_WrData(0x01);
		IERG3810_TFTLCD_WrData(0xDF);
	IERG3810_TFTLCD_WrReg(0x2c);
	IERG3810_TFTLCD_WrData(color);
}


void IERG3810_TFTLCD_FillSquare(u16 color, u16 start_x, u16 start_y)
{
	u32 index = 0;
	IERG3810_TFTLCD_WrReg(0x2A);
		IERG3810_TFTLCD_WrData(start_x>>8);
		IERG3810_TFTLCD_WrData(start_x & 0xFF);
		IERG3810_TFTLCD_WrData((8 + start_x - 1) >> 8);
		IERG3810_TFTLCD_WrData((8 + start_x - 1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2B);
		IERG3810_TFTLCD_WrData(start_y>>8);
		IERG3810_TFTLCD_WrData(start_y & 0xFF);
		IERG3810_TFTLCD_WrData((8 + start_y - 1) >> 8);
		IERG3810_TFTLCD_WrData((8 + start_y - 1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2c);
	for(index=0;index<64;index++)
	{
		IERG3810_TFTLCD_WrData(color);
	}
}

void Draw_playfield(u16 **playfield)
{-0
	int i = 0, j = 0;
	int color = 0;
	for(i = 0; i < 10; i++)
	{
		for(j = 0; j < 24; j++)
		{
			color = playfield[i][j];
			IERG3810_TFTLCD_FillSquare(color, 80+i*8, j*8+64);
		}
	}
}

u16 switch_color(u16 type)
{
	switch (type)
	{
		case 1:
			return 0XF800;
		case 2:
			return 0XFD20;
		case 3:
			return 0XFFE0;
		case 4:
			return 0X400;
		case 5:
			return 0X7FF;
		case 6:
			return 0X1F;
		case 7:
			return 0X8010;
		
		
	}
}