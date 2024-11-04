#include "stm32f10x.h"
#include "IERG3810_USART.h"

// put your procedure and code here
void IERG3810_USART1_init(u32 pclk1,u32 bound)
{
    //USART1
    float temp;
    u16 mantissa;
    u16 fraction;
	
    temp=(float)(pclk1*1000000)/(bound*16);
    mantissa = temp;
    fraction = (temp-mantissa)*16;
    mantissa <<= 4;
    mantissa += fraction;
	
		//RCC->APB2ENR |= 0x00004004;
    RCC->APB2ENR |= 1<<2;            //(RM0008 page-112)    GPIOA clock enable
    RCC->APB2ENR |= 1<<14;        //(RM0008 page-115) USART1 Enable
    GPIOA->CRH &= 0xFFFFF00F;    //set PA9,PA10 Alternate Function
    GPIOA->CRH |= 0x000008B0;    //set PA9,PA10 Alternate Function
    RCC->APB2RSTR |= 1<<14;        //(RM0008 page-106) USART1 RST
    RCC->APB2RSTR &= ~(1<<14);			//(RM0008 page-106) USART1 RST
    USART1->BRR = mantissa;        //(RM0008 page-820)
    USART1->CR1 |= 0x2008;        //(RM0008 page-821)
}

void IERG3810_USART2_init(u32 pclk1,u32 bound)
{
	//USART2
	float temp;
	u16 mantissa;
	u16 fraction;
	temp=(float)(pclk1*1000000)/(bound*16);
	mantissa = temp;
	fraction = (temp-mantissa)*16;
	mantissa <<= 4;
	mantissa += fraction;
	RCC->APB2ENR |= 1<<2;			//(RM0008 page-112)
	RCC->APB1ENR |= 1<<17;		//(RM0008 page-115)
	GPIOA->CRL &= 0xFFFF00FF;	//set PA2,PA3 Alternate Function
	GPIOA->CRL |= 0x00008B00;	//set PA2,PA3 Alternate Function
	RCC->APB1RSTR |= 1<<17;		//(RM0008 page-109)
	RCC->APB1RSTR &= ~(1<<17);
	USART2->BRR = mantissa;		//(RM0008 page-820)
	USART2->CR1 |= 0x2008;		//(RM0008 page-821)
}

void USART_print(u8 USARTport, char *st)
{
	u8 i=0;
	while (st[i] != 0x00)
	{
		if(USARTport == 1)
		{	
			USART1->DR = st[i];
			while(!(USART1->SR &= 0x0080));
		}
		if(USARTport == 2)
			{
			USART2->DR = st[i];
			while(!(USART2->SR &= 0x0080));
			}
		if(i==255)break;
		i++;
	}
}

void USART_print_int(u8 USARTport, u8 ipt)
{

		if(USARTport == 1)
		{	
			USART1->DR = ipt;
			while(!(USART1->SR &= 0x0080));
		}
		if(USARTport == 2)
			{
			USART2->DR = ipt;
			while(!(USART2->SR &= 0x0080));
			}
}
