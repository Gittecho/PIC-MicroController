/*
 * File:   switch.c
 * Author: NUBAISH AHMED
 *
 * Created on 23 August, 2022, 2:17 PM
 */


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

//frequency define
#define _XTAL_FREQ 6000000
//VARIABLE DECLARATION
unsigned char val;
        
void main()
{
    //MODE DEFINITION 
        TRISB = 0x0F; //C - INPUT MODE
        TRISD = 0x00; //D - OUTPUT MODE
    //PORT VALUE DEFINITION
        PORTB = 0x00;
        PORTD = 0x00;
        OPTION_REG = 0x7F;
        
    while (1)
    {
        //OBTAIN THE VALUE
        val = PORTB ; 
        
        switch (val)
        {
            case 0x0E:PORTD = 0x04; 
                break;
            case 0x0D:PORTD = 0x10;
                break;
            case 0x0B:PORTD = 0x00;
                break;
            case 0x07:PORTD = 0x14;
                break;   
            default:
                PORTD = 0X00; 
                break;
        }
             
    }
}



