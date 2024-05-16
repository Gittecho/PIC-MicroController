/*
 * File:   adc.c
 * Author: NUBAISH AHMED
 *
 * Created on 22 October, 2022, 7:13 PM
 */
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

void init();
void lcdcmd(unsigned char);
void lcddata(unsigned char);
void lcdout (unsigned int);

unsigned int j,m;
unsigned char k[10];
unsigned char highval,lowval,n;
unsigned long value,volt;

int main()
{
    
    init();
    while(1)
    {
        ADCON0 = 0x91; //ADC set to ON, Channel selection, Clock conversion(fosc 32)
        ADCON0 |=0x04; //To change the 3rd bit (1000 0101) to start the conversion 
        while (ADCON0 & 0x04); //checking until the process complete
        highval = ADRESH;
        lowval = ADRESL;
        value =((unsigned int)highval<<8)+(unsigned int)lowval; 
        volt= value * 100 / 1023;
        lcdcmd(0x80);
        lcdout(volt);
        __delay_ms(100);
        
    }
    
    
}

void init()
{
    TRISA = 0x04; 
    TRISB = 0x00;
    TRISD = 0x00;
    TRISC = 0x00;
    
    
    ADCON0=0x91; //ADC set to ON, Channel selection, Clock conversion(fosc 32)
    ADCON1=0x80; //justify right port config selection (Analog or digital)
    //LCD CONFIGURATION
    
    lcdcmd(0x30);
    __delay_ms(50);
    lcdcmd(0x30);
    __delay_ms(50);
    lcdcmd(0x30);
    __delay_ms(50);
    lcdcmd(0x38);
    __delay_ms(50);
    lcdcmd(0x06);
    __delay_ms(50);
    lcdcmd(0x0C);
    __delay_ms(50);
    lcdcmd(0x01);
    __delay_ms(50);
    
}

void lcdout(unsigned int i)
{
    unsigned char s;
    j=1;
    m=i;
    while(m!=0)
    {
        s= m-((m/10)*10);
        k[j]=s;
        j++;
        m=m/10;
    }
    k[j]='\0';
    j=j-1;
    
    if (j>2){
        lcddata(0x31);
        lcddata(0x30);
        lcddata(0x30);
    }
    
    else{
        lcddata(0x20);
    while(j!=0)
    {
        n=0x30+k[j];//conversion of number into decimal hex value
        lcddata(n);
        j--;
    }
        lcddata(0x56);
        
    }
    
    if(i > 0 && i < 25)
    {
        PORTB= 0x01;
    }
    if(i >25 && i < 50)
    {
        PORTB = 0x02;
    }
    if(i > 50 && i < 75)
    {
        PORTB = 0x00;
    }
     if(i > 75)
    {
        PORTB = 0x03;
    }
}

void lcdcmd(unsigned char i)
{
    PORTC &= ~0x08;   
    PORTD = i;        
    PORTC |= 0x01;    
    PORTC &= ~0x01;   
    __delay_ms(100);
}

void lcddata(unsigned char i)
{
    PORTC |= 0x08;   
    PORTD = i;        
    PORTC |= 0x01;    
    PORTC &= ~0x01;   
    __delay_ms(100);
}

