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
#define _XTAL_FREQ 16000000
#define RS RC2
#define EN RC1
#define EEPROM_Address_R 0xA1 // 1010 000 1 (slave read address)
#define EEPROM_Address_W 0xA0 // 1010 000 0 (slave write address)

/*
 >>>> Init function
 >>>> Wait function
 >>>> Start function
 >>>> Stop function
 >>>> Acknowledgment 
 >>>> Not Acknowledgment
 >>>> read and Write
 */
void I2C_Master_Init(const unsigned long baud);
void I2C_Master_Wait();
void I2C_Master_Start();
void I2C_Master_RepeatedStart();
void I2C_Master_Stop();
void I2C_ACK();
void I2C_NACK();
int I2C_Master_Write(unsigned char);
int I2C_Read_Byte(int ack);

void init(void);
void lcd_command(unsigned char);
void lcd_data(unsigned char);
int bcd_2_dec(int);
int dec_2_bcd(int);
 
void EEPROM_Write(unsigned int add, unsigned char data);
void EEPROM_Write_Page(unsigned int add, unsigned char* data, unsigned char len);
unsigned char EEPROM_Read(unsigned int add);
void EEPROM_Read_Page(unsigned int add, unsigned char* data, unsigned int len);

//variables used
char msg1[5]={"TIME:"};
char msg2[5]={"DATE:"};
int i,j,k,l;
int sec=55;
int min=59;
int hour=23;
int date=28;
int month=03;
int year =02;
char sec1,sec2,min1,min2,hour1,hour2,date1,date2,month1,month2,year1,year2;

// init function
void I2C_Master_Init(const unsigned long baud)
{
  SSPCON = 0b00101000; //serial port and master mode 5 and 3 
  SSPCON2 = 0;
  SSPADD = (_XTAL_FREQ/(4*baud))-1;
  SSPSTAT = 0;
  TRISC3 = 1; 
  TRISC4 = 1;
}

void I2C_Master_Wait() 
{
  //bit 2 - enable when transmit progress, Bit 5 - to check start stop and ack
  while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}
void I2C_Master_Start()
{
  I2C_Master_Wait();
  //enable start on SDA and SCL
  SEN = 1;  
}

void I2C_Master_RepeatedStart()
{
  I2C_Master_Wait();
  RSEN = 1; //enables Repeated mode start condition
}
void I2C_Master_Stop()
{
  I2C_Master_Wait();
  PEN = 1; //enable Stop condition
}

int I2C_Master_Write(unsigned char data) //0xA0 stored in data
{
  I2C_Master_Wait();
  SSPBUF = data; // data is stored in SSPBUF buffer
  while(!SSPIF); // SSPIF is 0 when progress
  SSPIF = 0; 
  return ACKSTAT;//0 - received 1 - Not received
}
int I2C_Read_Byte(int ack)
{

  I2C_Master_Wait();
  RCEN = 1; // Enable receive mode & Start Reception
  while(!SSPIF); // Wait Until Completion
  SSPIF = 0; // Clear The Interrupt Flag Bit
  I2C_Master_Wait();
  ACKDT=(ack)?0:1;
  ACKEN=1;
  return SSPBUF; // Return The Received Byte
}

// to send ack ACKEN = 1, ACK - ACKDT = 0, NACK - ACKDT = 1
void I2C_ACK(void)
{
  ACKDT = 0; // 0 for ACK
  I2C_Master_Wait();
  ACKEN = 1; // Send ACK
}
void I2C_NACK(void)
{
  ACKDT = 1; // 1 for NACK
  I2C_Master_Wait();
  ACKEN = 1; // Send NACK
}

/*
 
 TO Write = Start + slave Address + address + data + ACK + stop
 To Read  = Start + Slave Address + address + read address + read + NACK +stop
 
 */
void EEPROM_Write(unsigned int add, unsigned char data)
{
  I2C_Master_Start();
  while(I2C_Master_Write(EEPROM_Address_W)) //slave address=0xA0 for write
    I2C_Master_RepeatedStart();//if ACK no received then repeated mode starts
 
  I2C_Master_Write(add>>8);//0x0020 : MSB 00 is taken 
  I2C_Master_Write((unsigned char)add);//LSB  20 is taken
  I2C_Master_Write(data);//transferring data
  I2C_Master_Stop();//stop bit
}

unsigned char EEPROM_Read(unsigned int add)
{
  unsigned char Data; //local variable
  I2C_Master_Start();
 
  while(I2C_Master_Write(EEPROM_Address_W))//slave address 0xA0 for write
    I2C_Master_RepeatedStart();
 
  I2C_Master_Write(add>>8);//0x0020 : MSB 00 is taken 
  I2C_Master_Write((unsigned char)add);//LSB  23 is taken
  I2C_Master_RepeatedStart();
 
  I2C_Master_Write(EEPROM_Address_R);//slave address 0xA1 for read
  Data = I2C_Read_Byte(1);
  I2C_NACK();
  I2C_Master_Stop();//stop bit 
 
  return Data;
} 

int dec_2_bcd(int temp)
{ 
return ((temp/10)<<4)+(temp%10);
}
int bcd_2_dec(int temp)
{ 
return ((temp>>4)*10)+(temp&0x0F);
}

void init(void)
{
    TRISD=0x00;
    TRISC=0x18;
    PORTD=0x00;
    lcd_command(0x38);
      __delay_ms(5);
      lcd_command(0x38);
      __delay_ms(5);
      lcd_command(0x38);
      __delay_ms(5);
      lcd_command(0x38);
      __delay_ms(5);
      lcd_command(0x0C);
      __delay_ms(5);
      lcd_command(0x06);
      __delay_ms(5);
      lcd_command(0x01);
      __delay_ms(5);
}
void lcd_command(unsigned char i)
{
    RS=0;
    PORTD=i;
    EN=1;
    EN=0;
    __delay_ms(5);
}
void lcd_data(unsigned char i)
{
    RS=1;
    PORTD=i;
    EN=1;
    EN=0;
    __delay_ms(5);
}

void settingDT(void)
{
    I2C_Master_Start();
    I2C_Master_Write(0xD0); //device Address
    I2C_Master_Write(0); //base address
    
    //Write Time
    I2C_Master_Write(dec_2_bcd(sec));
    I2C_Master_Write(dec_2_bcd(min));
    I2C_Master_Write(dec_2_bcd(hour));
    
    I2C_Master_Write(1); //value instead of Day
    
    //Write Date
    I2C_Master_Write(dec_2_bcd(date));
    I2C_Master_Write(dec_2_bcd(month));
    I2C_Master_Write(dec_2_bcd(year));
    I2C_Master_Stop();
}

void updateDT(void)
{
    I2C_Master_Start();
    I2C_Master_Write(0xD0);
    I2C_Master_Write(0);
    I2C_Master_Stop();
            
    I2C_Master_Start(); //use restart s=instead
    I2C_Master_Write(0xD1);
    
    //Time Read
    sec  =(bcd_2_dec(I2C_Read_Byte(1)));
    min  =(bcd_2_dec(I2C_Read_Byte(1)));
    hour =(bcd_2_dec(I2C_Read_Byte(1)));

    (I2C_Read_Byte(1)); //Day read
    
    //Date Read
    date=(bcd_2_dec(I2C_Read_Byte(1)));
    month=(bcd_2_dec(I2C_Read_Byte(1)));
    year=(bcd_2_dec(I2C_Read_Byte(1)));
    I2C_Master_Stop();
    
    I2C_Master_Start();
    I2C_Master_Write(0xD1);
    I2C_Read_Byte(1);
    I2C_Master_Stop();
}
void main(void) {
    
    init();
    I2C_Master_Init(100);
    settingDT();
    while(1)
    {
        updateDT(); //read the updated value
        
        sec1=sec/10;
        sec2=sec%10;
        min1=min/10;
        min2=min%10;
        hour1=hour/10;
        hour2=hour%10;
        date1=date/10;
        date2=date%10;
        month1=month/10;
        month2=month%10;
        year1=year/10;
        year2=year%10;
        
        lcd_command(0x80);
        for(i=0;i<5;i++)
        {
            lcd_data(msg1[i]);
        }
        lcd_data(hour1+'0');
        lcd_data(hour2+'0');
        lcd_data(0x2D);
        lcd_data(min1+'0');
        lcd_data(min2+'0');
        lcd_data(0x2D);
        lcd_data(sec1+'0');
        lcd_data(sec2+'0');
        
        lcd_command(0xC0);
        for(i=0;i<5;i++)
        {
            lcd_data(msg2[i]);
        }
        lcd_data(date1+'0');
        lcd_data(date2+'0');
        lcd_data(0x2D);
        lcd_data(month1+'0');
        lcd_data(month2+'0');
        lcd_data(0x2D);
        lcd_data(year1+'0');
        lcd_data(year2+'0');
        
    __delay_ms(500);
    }

}
