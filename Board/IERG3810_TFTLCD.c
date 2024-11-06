#include "stm32f10x.h"
#include "IERG3810_TFTLCD.h"
#include "IERG3810_clock.h"
#include "FONT.h"

typedef struct
{
	u16 LCD_REG;
	u16 LCD_RAM;
} LCD_TypeDef;

#define LCD_BASE ((u32)(0x6C000000 | 0x000007FE))
#define LCD ((LCD_TypeDef *) LCD_BASE)

const unsigned char chi_1616[3][32]={
	{0x00,0x02,0x00,0x0C,0x7F,0xF0,0x40,0x40,0x5A,0x55,0x54,0x95,0x50,0xA5,0x5F,0x2A,0xC2,0x4A,0x51,0x12,0x5B,0x24,0x54,0x84,0x50,0x88,0x5E,0x40,0x40,0x40,0x00,0x00},	//LIEU
	{0x00,0x00,0x47,0xC0,0x44,0x42,0x44,0x41,0x44,0x42,0x7C,0x7C,0x00,0x08,0x00,0x1C,0x00,0xE8,0x07,0x08,0xF8,0x08,0x00,0x08,0x00,0x48,0x00,0x38,0x00,0x0E,0x00,0x00},	//WANG
	{0x00,0x41,0x10,0x42,0x10,0x8C,0x11,0x00,0x12,0x00,0x14,0x08,0x18,0x06,0xFF,0xE0,0x18,0x08,0x14,0x06,0x12,0x00,0x11,0x00,0x10,0x88,0x10,0x47,0x00,0x40,0x00,0x00},	//KIT
};

// put your procedure and code here

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

void IERG3810_TFTLCD_FillRectangle(u16 color, u16 start_x, u16 length_x, u16 start_y, u16 length_y)
{
	u32 index = 0;
	IERG3810_TFTLCD_WrReg(0x2A);
		IERG3810_TFTLCD_WrData(start_x>>8);
		IERG3810_TFTLCD_WrData(start_x & 0xFF);
		IERG3810_TFTLCD_WrData((length_x + start_x - 1) >> 8);
		IERG3810_TFTLCD_WrData((length_x + start_x - 1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2B);
		IERG3810_TFTLCD_WrData(start_y>>8);
		IERG3810_TFTLCD_WrData(start_y & 0xFF);
		IERG3810_TFTLCD_WrData((length_y + start_y - 1) >> 8);
		IERG3810_TFTLCD_WrData((length_y + start_y - 1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2c);
	for(index=0;index<length_x * length_y;index++)
	{
		IERG3810_TFTLCD_WrData(color);
	}
}

void IERG3810_TFTLCD_SevenSegment(u16 color, u16 start_x, u16 start_y, u8 digit)
{
	/*
	IERG3810_TFTLCD_FillRectangle(color,start_x + 10,55,start_y + 130,10);	//a
	IERG3810_TFTLCD_FillRectangle(color,start_x + 10,55,start_y + 65,10);	//g
	IERG3810_TFTLCD_FillRectangle(color,start_x + 10,55,start_y,10);	//d
	IERG3810_TFTLCD_FillRectangle(color,start_x,10,start_y + 75,55);	//f
	IERG3810_TFTLCD_FillRectangle(color,start_x + 65,10,start_y + 75,55);	//b
	IERG3810_TFTLCD_FillRectangle(color,start_x,10,start_y + 10,55);	//e
	IERG3810_TFTLCD_FillRectangle(color,start_x + 65,10,start_y + 10,55);	//c
	*/
	if(digit != 1 && digit != 4){
		IERG3810_TFTLCD_FillRectangle(color,start_x + 10,55,start_y + 130,10);	//a
	}
	if(digit != 5 && digit != 6){
		IERG3810_TFTLCD_FillRectangle(color,start_x + 65,10,start_y + 75,55);	//b
	}
	if(digit != 2){
		IERG3810_TFTLCD_FillRectangle(color,start_x + 65,10,start_y + 10,55);	//c
	}
	if(digit != 1 && digit != 4 && digit != 7){
		IERG3810_TFTLCD_FillRectangle(color,start_x + 10,55,start_y,10);	//d
	}
	if(digit == 0 || digit == 2 || digit == 6 || digit == 8){
		IERG3810_TFTLCD_FillRectangle(color,start_x,10,start_y + 10,55);	//e
	}
	if(digit != 1 && digit != 2 && digit != 3 && digit != 7){
		IERG3810_TFTLCD_FillRectangle(color,start_x,10,start_y + 75,55);	//f
	}
	if(digit != 1 && digit != 0 && digit != 7){
		IERG3810_TFTLCD_FillRectangle(color,start_x + 10,55,start_y + 65,10);	//g
	}
}

void IERG3810_TFTLCD_ShowChar(u16 x, u16 y, u8 ascii, u16 color, u16 bgcolor)
{
	u8 i, j;
	u8 index;
	u8 height = 16, length = 8;
	if (ascii<32 || ascii >127) return;
	ascii -=32;
	IERG3810_TFTLCD_WrReg(0x2A);
		IERG3810_TFTLCD_WrData(x>>8);
		IERG3810_TFTLCD_WrData(x & 0xFF);
		IERG3810_TFTLCD_WrData((length + x - 1) >> 8);
		IERG3810_TFTLCD_WrData((length + x - 1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2B);
		IERG3810_TFTLCD_WrData(y>>8);
		IERG3810_TFTLCD_WrData(y & 0xFF);
		IERG3810_TFTLCD_WrData((height + y - 1) >> 8);
		IERG3810_TFTLCD_WrData((height + y - 1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2c);

	
	
	for (j = 0;j<height/8;j++)
	{
		for(i = 0;i<height/2;i++)
		{
			for(index = 0;index < length; index++)
			{
				if ((asc2_1608[ascii][index*2+1-j]>>i) & 0x01) IERG3810_TFTLCD_WrData(color);
				else IERG3810_TFTLCD_WrData(bgcolor);
			}
		}
	}
}

void IERG3810_TFTLCD_ShowChinese(u16 x, u16 y, u16 color, u16 bgcolor)
{
	u8 i, j,cat;
	u8 index;
	u8 height = 16, length = 48;
	

	IERG3810_TFTLCD_WrReg(0x2A);
		IERG3810_TFTLCD_WrData(x>>8);
		IERG3810_TFTLCD_WrData(x & 0xFF);
		IERG3810_TFTLCD_WrData((length + x - 1) >> 8);
		IERG3810_TFTLCD_WrData((length + x - 1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2B);
		IERG3810_TFTLCD_WrData(y>>8);
		IERG3810_TFTLCD_WrData(y & 0xFF);
		IERG3810_TFTLCD_WrData((height + y - 1) >> 8);
		IERG3810_TFTLCD_WrData((height + y - 1) & 0xFF);
	IERG3810_TFTLCD_WrReg(0x2c);

	
	for (j = 0;j<height/8;j++)
	{
		for(i = 0;i<height/2;i++)
		{
			for(index = 0;index < length; index++)
			{
				if ((chi_1616[cat][index*2+1-j]>>i) & 0x01) IERG3810_TFTLCD_WrData(color);
				else IERG3810_TFTLCD_WrData(bgcolor);
			}
		}
	}
}

u16 IERG3810_TFTLCD_ReadColor(u16 x, u16 y)
{
	u16 tmpcolor = 0;
	u16 dummy;
IERG3810_TFTLCD_WrReg(0x2A); // 设置列地址
IERG3810_TFTLCD_WrData(x >> 8);
IERG3810_TFTLCD_WrData(x & 0xFF);

IERG3810_TFTLCD_WrReg(0x2B); // 设置行地址
IERG3810_TFTLCD_WrData(y >> 8);
IERG3810_TFTLCD_WrData(y & 0xFF);

IERG3810_TFTLCD_WrReg(0x2E); // 读取内存
dummy = IERG3810_TFTLCD_RdData(); // 伪读数
color = IERG3810_TFTLCD_RdData(); // 实际颜色值

return color;
	
}
