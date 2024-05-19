/*
 * File:   keypad.c
 * Author: NUBAISH AHMED
 *
 * Created on 28 November, 2022, 10:01 PM
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
#include "lcd.h"
#define _XTAL_FREQ 6000000

#define column1 RB0
#define column2 RB1
#define column3 RB2
#define row1 RB4
#define row2 RB5
#define row3 RB6
#define row4 RB7

#define Keypad_PORT          PORTB
#define Keypad_PORT_Direction     TRISB

void keypad (void);
void InitKeypad (void);
unsigned char arr[10] = {"KEY VALUE"};

void main(){
    
    InitKeypad();
    lcdinit();                
    lcdCmd(0x80);
    for(unsigned char i=0;i<20;i++)       //loop through letters in name untill less than 1
    {
        lcdData(arr[i]);      //calling lcddata func 
    }
    while(1)
    {
        keypad();
    }
}

void keypad()
 {
        column1=0,column2=1,column3=1;
        if(row1==0){lcdCmd(0xc5);lcdData('1');while(row1==0);}
        if(row2==0){lcdCmd(0xc5);lcdData('4');while(row2==0);}
        if(row3==0){lcdCmd(0xc5);lcdData('7');while(row3==0);}
        if(row4==0){lcdCmd(0xc5);lcdData('*');while(row4==0);}
        
        column1=1,column2=0,column3=1;
        if(row1==0){lcdCmd(0xc5);lcdData('2');while(row1==0);}
        if(row2==0){lcdCmd(0xc5);lcdData('5');while(row2==0);}
        if(row3==0){lcdCmd(0xc5);lcdData('8');while(row3==0);}
        if(row4==0){lcdCmd(0xc5);lcdData('0');while(row4==0);}
        
        column1=1,column2=1,column3=0;
        if(row1==0){lcdCmd(0xc5);lcdData('3');while(row1==0);}
        if(row2==0){lcdCmd(0xc5);lcdData('6');while(row2==0);}
        if(row3==0){lcdCmd(0xc5);lcdData('9');while(row3==0);}
        if(row4==0){lcdCmd(0xc5);lcdData('#');while(row4==0);}   
        
 }
void InitKeypad(void)
{
            Keypad_PORT                = 0x00;        // Set Keypad port pin values zero
            Keypad_PORT_Direction = 0xF0;             // Last 4 pins input, First 4 pins output        
            OPTION_REG &= 0x7F;
}


