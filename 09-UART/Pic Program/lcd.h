
//
//#include <xc.h>
//#define _XTAL_FREQ 6000000

void init(void);
void lcdCmd(unsigned char);
void lcdData(unsigned char);
void delay(unsigned int);
void lcdOut(unsigned int);
unsigned char j,k[10],Equal,plus,minus;
unsigned int num1,num2,sum,delaycount,m,n,select;


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
