
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
#define _XTAL_FREQ 600000

void LCD_data(unsigned char);           
void LCD_command(unsigned char);        
void LCD_output(unsigned int);          
void keyscane();
void init();
unsigned char array[20] = {"BATTERY=  . %"};
unsigned char lb[20] = {"VOLTAGE LOW   "};
unsigned char nb[20] = {"VOLTAGE NORMAL"};
unsigned char hb[20] = {"VOLTAGE HIGH  "};
unsigned char x, m, n, value;              
unsigned int d3, d2, d1, j, i;

void main()                             
{
    init();
    while (1) {
        keyscane();                     

    }
}

void init() {                    
    TRISC = 0x00;                
    TRISB = 0xF0;                
    TRISD = 0x00;                
    OPTION_REG &= 0x7F;          
    LCD_command(0x38);          
    __delay_ms(100);  
    LCD_command(0x38);          
    __delay_ms(100);
    LCD_command(0x38);          
    __delay_ms(100);
    LCD_command(0x06);          
    __delay_ms(100);            
    LCD_command(0x0C);           
    __delay_ms(100);
    LCD_command(0x01);           
    __delay_ms(100);
    j = 155;                    

}

void keyscane() {              
    PORTB &= 0xF0;              
    value = PORTB;
    switch (value) {           
        case 0xE0:
            LCD_command(0x80);  
            for (x = 0; x < 8; x++) {
                LCD_data(array[x]);
            }
            LCD_command(0x8C);  
            LCD_data(array[12]);
            LCD_command(0x8A);
            LCD_data(array[10]);
            LCD_command(0x88);
            LCD_output(j);
            break;
        case 0xD0:
            j++;                 
            if (j > 225) {       
                j = 225;
            }
            LCD_command(0x88);
            LCD_output(j);

            if (j < 175) { 
                //LOW voltage 
                PORTB |= 0x02;
                LCD_command(0xC0);
                for (x = 0; x < 14; x++) {
                    LCD_data(lb[x]);
                }
            } else if (j > 205) {     
                // high voltage
                PORTB |= 0x08;
                LCD_command(0xC0);
                for (x = 0; x < 14; x++) {

                    LCD_data(hb[x]);
                }

            } else {
                //Normal voltage
                PORTB|=0x04;
                LCD_command(0xC0);
                for (x = 0; x < 14; x++) {
                    LCD_data(nb[x]);
                }
            }

            break;
        case 0xB0:
            j--;                 
            if (j < 155) {       
                j = 155;
            }
            LCD_command(0x88);
            LCD_output(j);
            if (j < 175) { 
                //low voltage
                PORTB|=0x02;
                LCD_command(0xC0);
                for (x = 0; x < 14; x++) {
                    LCD_data(lb[x]);
                }
            } else if (j > 205) {   //HIGH VOLTAGE
                PORTB|=0x08;
                LCD_command(0xC0);
                for (x = 0; x < 14; x++) {

                    LCD_data(hb[x]);
                }

            } else {
                //NORMAL VOLTAGE
                PORTB |= 0x04;
                LCD_command(0xC0);
                for (x = 0; x < 14; x++) {
                    LCD_data(nb[x]);
                }
            }
            break;
        case 0x70:
            j = 155;
            LCD_command(0x88);
            LCD_output(j);
            if (j < 175) {
                LCD_command(0xC0);
                for (x = 0; x < 14; x++) {
                    LCD_data(lb[x]);
                }
            } else if (j > 205) {
                LCD_command(0xC0);
                for (x = 0; x < 14; x++) {

                    LCD_data(hb[x]);
                }

            } else {
                LCD_command(0xC0);
                for (x = 0; x < 14; x++) {
                    LCD_data(nb[x]);
                }
            }
            break;
        default:
            break;
    }

}

void LCD_command(unsigned char i) {
    PORTC &= ~0x08;   
    PORTD = i;        
    PORTC |= 0x01;    
    PORTC &= ~0x01;   
    __delay_ms(100);
}

void LCD_data(unsigned char i) {
    PORTC |= 0x08;   
    PORTD = i;
    PORTC |= 0x01;   
    PORTC &= ~0x01;  
    __delay_ms(10);
}

void LCD_output(unsigned int i)                        
{ 
    d3 = (unsigned char) (i / 100);                   
    d2 = (unsigned char) ((i - (d3 * 100)) / 10);  
    d1 = (unsigned char) (i - ((d3 * 100)+(d2 * 10)));
    LCD_command(0x88);                                
    LCD_data(0x30 + d3);                              
    LCD_command(0x89);
    LCD_data(0x30 + d2);
    LCD_command(0x8B);
    LCD_data(0x30 + d1);

}