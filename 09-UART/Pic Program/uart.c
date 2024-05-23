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
//#include "lcd.h"

//Master code

void init(void);
void lcdData(unsigned char);
void lcdCmd(unsigned char);
void delay(unsigned int);
unsigned char val,Mdata;
unsigned char sen1[15] = {"SENDING..."};
unsigned char sen2[15] = {"RECEIVED.."};


void main(){
    
    init();
    lcdCmd(0x80);
    for(int i=0;i<14;i++)
    {
        lcdData(sen1[i]);
    }

    while(1)
    {

        val = PORTB;
        switch(val)
        {
            case 0xE0:
                TXREG = 'A';

                break;
            case 0xB0:
                TXREG = 'B';

                break;
            case 0xD0:
                TXREG = 'C';

                break;
            case 0x70:
                TXREG = 'D';

                break;
        }
       if(PIR1 & 0X20)  //checking for  PIR1 to high
        {
            lcdCmd(0x01);
            for(int i=0;i<14;i++)
            {
                lcdData(sen1[i]);
            }
            lcdCmd(0xC0);
            for(int i=0;i<14;i++)
            {
                lcdData(sen2[i]);
            }
        
            Mdata = RCREG;
            lcdCmd(0xCD);
            lcdData(Mdata);
            delay(3000);
        
            INTCON &= 0XFE;
        
        }
    }


}

void init()
{
    
    //LCD configuration
    TRISB = 0xF0;
    PORTB = 0x00;
    OPTION_REG &= 0x7F;
    TRISD=0x00;
    TRISC = 0x80;
    PORTD=0X00;
 
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
    
    
    //uart configuration
    TXSTA = 0x20; //TX enable 0010 0000
    RCSTA = 0x90; //SPEN enable and 8 bit selection 1001 0000
    SPBRG = 0x09; //baud rate 0000 1001
    
    //interrupt configuration
    INTCON = 0xC0;
}


//lCD DATA function
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

void delay(unsigned int delaycount)
{
    while(--delaycount);
}

void __interrupt() ISR()  //initiating interrupt function
{ 
    while(PIR1 & 0X20)  //checking for  PIR1 to high
    {
        lcdCmd(0xC0);
        for(int i=0;i<14;i++)
        {
            lcdData(sen1[i]);
        }
        
        Mdata = RCREG;
        lcdCmd(0xCD);
        lcdData(Mdata);
        delay(3000);
        
        INTCON &= 0XFE;
        
    }
    
}

