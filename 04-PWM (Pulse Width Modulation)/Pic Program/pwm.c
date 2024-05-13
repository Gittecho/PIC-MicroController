
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

//declaration
void init(void);
void pwmchange(void);

unsigned char ph20,pl20; 
unsigned char ph60,pl60; 
unsigned char ph90,pl90; 

void main(void)
{
init();
while(1)
{
    pwmchange();
}
}
void init()
{
    
TRISC=0xFB;  //RC2 - OUTPUT
CCP1CON=0x3C; //Enable PWM mode
CCPR1L=0x2E;  
T2CON=0x06;  //Timer control (pre scale and timer select)
PR2=0x2E; 

//High and Low value (By calculation))

ph20=0x09; //high 20 DC
pl20=0x20; //Low  20 DC 
ph60=0x1C; //high 60 DC
pl60=0x01; //Low  60 DC
ph90=0x2A; //high 90 DC 
pl90=0x10; //Low  90 DC
}
void pwmchange()
{
    CCPR1L=ph20;  
    CCP1CON=(CCP1CON&0xCF)|pl20; 
    __delay_ms(2000);
    CCPR1L=ph60; 
    CCP1CON=(CCP1CON&0xCF)|pl60; 
    __delay_ms(2000);
    CCPR1L=ph90;   
    CCP1CON=(CCP1CON&0xCF)|pl90; 
    __delay_ms(2000);
}