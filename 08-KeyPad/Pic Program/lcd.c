#include <xc.h>
#define _XTAL_FREQ 6000000

void delay(unsigned int delaycount)
{
    while(--delaycount);
}

//define LCD DATA function
void lcdData(unsigned char a)  
{
PORTC |=0x08;
PORTD=a;
PORTC |=0x01;
PORTC &=~0x01;
delay(100);
}

//define LCD CMD function
void lcdCmd(unsigned char a)   
{
PORTC &=0xF7;
PORTD=a;
PORTC|=0x01;
PORTC&=0x00;
delay(100);
}

void lcdinit(void)
{
    TRISC=0x00;
    TRISD=0x00;

    lcdCmd(0x38);
    delay(100); 
    lcdCmd(0x38);
    delay(100); 
    lcdCmd(0x38);
    delay(100); 
    lcdCmd(0x38);
    delay(100); 
    lcdCmd(0x06);
    delay(100); 
    lcdCmd(0x0C);
    delay(100); 
    lcdCmd(0x01);
    delay(100);  
}
