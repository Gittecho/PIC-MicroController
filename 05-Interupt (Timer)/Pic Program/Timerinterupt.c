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

unsigned char count;  
unsigned char segments_code[10] = {0xFC,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xF6};
unsigned char counter=0;

int main(void)
{

    INTCON|=0XA0; 
    
    TRISB = 0x00; // Set All Pins To Be Output Pins
    PORTB = 0x00; // Initially Clear All The 8-Pins
    
    TRISC=0X00;
    PORTC=0X00;
    
    OPTION_REG &= 0x07;
    
    while(1)
  {
    PORTB = segments_code[counter];
    __delay_ms(500);
    counter++;
    if(counter==10)
       counter=0;
  }
 
}


//main interrupt function 
void __interrupt() ISR()         
{
    if(INTCON & 0x04)       
    {
        count++;
        if (count == 23)
        {
            PORTC = ~PORTC;
            count = 0;
            
        }
        
       INTCON&=0xFB;     
    }
}