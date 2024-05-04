
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

void init(void);
void lcdCmd(unsigned char);
void lcdData(unsigned char);
void delay(unsigned int);
void lcdOut(unsigned int);
unsigned char j,k[10],Equal,plus,minus;
unsigned int num1,num2,sum,delaycount,m,n,select;

void main()
{
    init();
    
    num1=202;
    num2=101;
    Equal='=';
    plus='+';
    minus = '-';
    
    lcdCmd(0x80);
    //for numbers lCDOUT
    lcdOut(num1); // number1 to LCD
    
    lcdCmd(0x84);
    //for numbers lCDOUT
    lcdOut(num2); //number2 to LCD
    

    lcdCmd(0x83);
    lcdData(minus); //sign to LCD
    
    lcdCmd(0x87);
    lcdData(Equal);//Equal to LCD
    
    
    
    if (num1>num2)
    {
       lcdCmd(0x88);
       lcdData(plus);
       lcdCmd(0x89);
       //for numbers lCDOUT
       sum=num1-num2;
           
       lcdOut(sum);  
    }
    else
    {
       lcdCmd(0x88);
       lcdData(minus);
       lcdCmd(0x89);
       //for numbers lCDOUT
       sum = num2-num1;
       lcdOut(sum);  
    }
        
    
    
    while(1);
}
void init(void)
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

void lcdOut(unsigned int i)
{
    unsigned char s,j=1;
    m=i;
    
    while(m!=0)
    {
        s=m-((m/10)*10);
        k[j]=s;
        j++;
        m=m/10;
    }
    k[j]='\0';
    j = j-1;
    while(j!=0)
    {
        n=0x30+k[j];//conversion of number into decimal hex value
        lcdData(n);
        j--;
    }
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

void delay(unsigned int delaycount)
{
    while(--delaycount);
}