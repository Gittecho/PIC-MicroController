
// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 6000000
//Function declaration 
void init();
void LcdCmd(char);
void LcdData(char);

//Variable declaration
char i,a;
char s = '*';
//char p[16]={"0x80","0x81","0x82","0x83","0x84","0x85","0x86","0x87","0x88","0x89","0x8A","0x8B","0x8C","0x8D","0x8E","0x8F"};
char t[9]={"THANK YOU!"};
char array[14]={"NUBAISH AHAMED"};

//main function
void main()
{
    init();
    LcdCmd(0x81);
    for(i=0;i<14;i++)
    {
        LcdData(array[i]);
    }
    LcdCmd(0xCE);
    LcdCmd(0x04);
    __delay_ms(100);
    for(i=0;i<14;i++)
    {
        LcdData(array[i]);
    }
    __delay_ms(1000);
    LcdCmd(0x01);
    LcdCmd(0x80);
    for(i=0;i<16;i++)
    {
        LcdData(s);
    }
    LcdCmd(0xC0);
    for(i=0;i<16;i++)
    {
        LcdData(s);
    }
    LcdCmd(0x01);
    
    while (1)
    {
        LcdCmd(0x84);
        for(i=0;i<10;i++)
        {
        LcdData(t[i]);
        }
        LcdCmd(0x01);
        
        
    }
    
// ROLLING DISPLAY PROGRAM
    
//    int j = 0;
//    for(j=0;j<16;j++)
//    {
//        LcdCmd(p[j]);
//        for(i=0;i<9;i++)
//        {
//        LcdData(t[i]);
//        }
//        LcdCmd(0x05);
//
//    }
    
    while(1);
}

//Init function
void init()
{
    TRISC=0x00;
    TRISD=0x00;
    LcdCmd(0x30);
    __delay_ms(50); 
    LcdCmd(0x30);
    __delay_ms(50); 
    LcdCmd(0x30);
    __delay_ms(50); 
    LcdCmd(0x38);
    __delay_ms(50); 
    LcdCmd(0x06);
    __delay_ms(50); 
    LcdCmd(0x0C);
    __delay_ms(50); 
    LcdCmd(0x01);
    __delay_ms(50);  
}

//define LCD DATA function
void LcdData(char a)  
{
PORTC |=0x08;
PORTD=a;
PORTC |=0x01;
PORTC &=~0x01;
__delay_ms(50);
}

//define LCD CMD function
void LcdCmd(char a)   
{
PORTC=~0x08;
PORTD=a;
PORTC|=0x01;
PORTC&=0x00;
__delay_ms(50);
}
