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
unsigned char I2C_Master_Write(unsigned char);
unsigned char I2C_Read_Byte(void);
 
void EEPROM_Write(unsigned int add, unsigned char data);
void EEPROM_Write_Page(unsigned int add, unsigned char* data, unsigned char len);
unsigned char EEPROM_Read(unsigned int add);
void EEPROM_Read_Page(unsigned int add, unsigned char* data, unsigned int len);


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

unsigned char I2C_Master_Write(unsigned char data) //0xA0 stored in data
{
  I2C_Master_Wait();
  SSPBUF = data; // data is stored in SSPBUF buffer
  while(!SSPIF); // SSPIF is 0 when progress
  SSPIF = 0; 
  return ACKSTAT;//0 - received 1 - Not received
}
unsigned char I2C_Read_Byte(void)
{

  I2C_Master_Wait();
  RCEN = 1; // Enable receive mode & Start Reception
  while(!SSPIF); // Wait Until Completion
  SSPIF = 0; // Clear The Interrupt Flag Bit
  I2C_Master_Wait();
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
  Data = I2C_Read_Byte();
  I2C_NACK();
  I2C_Master_Stop();//stop bit 
 
  return Data;
} 

void main(void) {
  //baud rate as standard mode
  I2C_Master_Init(100000); 
  unsigned int Address1 = 0x0023; 
  unsigned int Address2 = 0x0028; 
  unsigned int Address3 = 0x0036;
  unsigned char Data = 0x41;     // 0x41 as Data To Write
 
  EEPROM_Write(Address1, Data++); // Write 0x41 > 0x0023
  __delay_ms(1000);
  EEPROM_Write(Address2, Data++); // Write 0x42 > 0x0028
  __delay_ms(1000);
  EEPROM_Write(Address3, Data);     // Write 0x43 > 0x0036
  __delay_ms(1000);
 
  Address1 = 0x0023; // Point To First Byte Location
  TRISD = 0x00;//make all pins are output in PortD
  
  PORTD = EEPROM_Read(Address1); // Read 0x41
  __delay_ms(1000);
  PORTD = EEPROM_Read(Address2); //  Read 0x42
  __delay_ms(1000);
  PORTD = EEPROM_Read(Address3);   // Read 0x43
  __delay_ms(1000);
 while(1)
{}
  return;
}