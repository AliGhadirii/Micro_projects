/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
� Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 1/3/2021
Author  : 
Company : 
Comments: 


Chip type               : ATmega32
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 512
*******************************************************/

#include <mega32.h>
#include <io.h>
#include <delay.h>
#include <stdio.h>
#include <interrupt.h>
#include <string.h>
#include <stdlib.h>


#define LCD_DPRT PORTA
#define LCD_DDDR DDRA
#define LCD_DPIN PINA
#define LCD_CPRT PORTB
#define LCD_CDDR DDRB
#define LCD_CPIN PINB
#define LCD_RS 2
#define LCD_RW 1
#define LCD_EN 0 

#define KEY_PRT PORTC   //initializing keyboard PORT  
#define KEY_DDR DDRC  //initializing keyboard DDR  
#define KEY_PIN PINC        //initializing keyboard PIN 


unsigned char * keypad [4][3]= {"1", "2", "3",  
                                "4", "5", "6",  
                                "7", "8", "9",  
                                "*", "0", "#"};   
                  
unsigned char colloc, rowlock;

 

 
void lcdCommand( unsigned char cmnd ) 
{
    LCD_DPRT = cmnd; 
    LCD_CPRT &= ~(1<<LCD_RS); 
    LCD_CPRT &= ~(1<<LCD_RW); 
    LCD_CPRT |= (1<<LCD_EN); 
    delay_us(1); 
    LCD_CPRT &= ~(1<<LCD_EN); 
    delay_us(100);
} 
 
void lcdData( unsigned char data )
{
    LCD_DPRT = data; 
    LCD_CPRT |= (1<<LCD_RS); 
    LCD_CPRT &= ~(1<<LCD_RW); 
    LCD_CPRT |= (1<<LCD_EN); 
    delay_us(1); 
    LCD_CPRT &= ~(1<<LCD_EN);
    delay_us(100); 
}
 
void lcd_init() 
{
    LCD_DDDR = 0xFF; 
    LCD_CDDR = 0xFF; 
    LCD_CPRT &= ~(1<<LCD_EN); 
    delay_us(2000); 
    lcdCommand(0x38); 
    lcdCommand(0x0E); 
    lcdCommand(0x01); 
    delay_us(2000); 
    lcdCommand(0x06); 
} 

void lcd_gotoxy(unsigned char x, unsigned char y)
{ 
    unsigned char firstCharAdr[] = {0x80,0xCD,0x94,0xD4};
    lcdCommand(firstCharAdr[y-1] + x - 1); 
    delay_us(100);
} 
 
void lcd_print( char * str ) 
{ 
    unsigned char i = 0;
    while(str[i] != 0)
    {
        lcdData(str[i]); 
        i++ ;
    }
}


void main(void)
{
    // Declare your local variables here  
    int flag_ring = 0; 
    // Input/Output Ports initialization
    // Port A initialization
    // Function: Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=Out 
    DDRA=(1<<DDA7) | (1<<DDA6) | (1<<DDA5) | (1<<DDA4) | (1<<DDA3) | (1<<DDA2) | (1<<DDA1) | (1<<DDA0);
    // State: Bit7=0 Bit6=0 Bit5=0 Bit4=0 Bit3=0 Bit2=0 Bit1=0 Bit0=0 
    PORTA=(0<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (0<<PORTA4) | (0<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);

    // Port B initialization
    // Function: Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=Out Bit2=Out Bit1=Out Bit0=Out 
    DDRB=(1<<DDB7) | (1<<DDB6) | (1<<DDB5) | (1<<DDB4) | (1<<DDB3) | (1<<DDB2) | (1<<DDB1) | (1<<DDB0);
    // State: Bit7=0 Bit6=0 Bit5=0 Bit4=0 Bit3=0 Bit2=0 Bit1=0 Bit0=0 
    PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);
    
    KEY_DDR = 0xF0;
    KEY_PRT = 0xFF;

    // Port D initialization
    // Function: Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=In Bit2=In Bit1=In Bit0=In 
    DDRD=(1<<DDD7) | (1<<DDD6) | (1<<DDD5) | (1<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
    // State: Bit7=0 Bit6=0 Bit5=0 Bit4=0 Bit3=T Bit2=T Bit1=T Bit0=T 
    PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

    // Timer/Counter 0 initialization
    // Clock source: System Clock
    // Clock value: Timer 0 Stopped
    // Mode: Normal top=0xFF
    // OC0 output: Disconnected
    TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (0<<CS01) | (0<<CS00);
    TCNT0=0x00;
    OCR0=0x00;

    // Timer/Counter 1 initialization
    // Clock source: System Clock
    // Clock value: Timer1 Stopped
    // Mode: Normal top=0xFFFF
    // OC1A output: Disconnected
    // OC1B output: Disconnected
    // Noise Canceler: Off
    // Input Capture on Falling Edge
    // Timer1 Overflow Interrupt: Off
    // Input Capture Interrupt: Off
    // Compare A Match Interrupt: Off
    // Compare B Match Interrupt: Off
    TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
    TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
    TCNT1H=0x00;
    TCNT1L=0x00;
    ICR1H=0x00;
    ICR1L=0x00;
    OCR1AH=0x00;
    OCR1AL=0x00;
    OCR1BH=0x00;
    OCR1BL=0x00;

    // Timer/Counter 2 initialization
    // Clock source: System Clock
    // Clock value: Timer2 Stopped
    // Mode: Normal top=0xFF
    // OC2 output: Disconnected
    ASSR=0<<AS2;
    TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (0<<CS22) | (0<<CS21) | (0<<CS20);
    TCNT2=0x00;
    OCR2=0x00;

    // Timer(s)/Counter(s) Interrupt(s) initialization
    TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);

    // External Interrupt(s) initialization
    // INT0: Off
    // INT1: Off
    // INT2: Off
    MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
    MCUCSR=(0<<ISC2);

    // USART initialization
    // USART disabled
    UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (0<<RXEN) | (0<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);

    // Analog Comparator initialization
    // Analog Comparator: Off
    // The Analog Comparator's positive input is
    // connected to the AIN0 pin
    // The Analog Comparator's negative input is
    // connected to the AIN1 pin
    ACSR=(1<<ACD) | (0<<ACBG) | (0<<ACO) | (0<<ACI) | (0<<ACIE) | (0<<ACIC) | (0<<ACIS1) | (0<<ACIS0);
    SFIOR=(0<<ACME);

    // ADC initialization
    // ADC disabled
    ADCSRA=(0<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (0<<ADPS1) | (0<<ADPS0);

    // SPI initialization
    // SPI disabled
    SPCR=(0<<SPIE) | (0<<SPE) | (0<<DORD) | (0<<MSTR) | (0<<CPOL) | (0<<CPHA) | (0<<SPR1) | (0<<SPR0);

    // TWI initialization
    // TWI disabled
    TWCR=(0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWEN) | (0<<TWIE);
    
    

    lcd_init();
    lcd_gotoxy(1,1);
      
    //GICR = 1<<INT0;		
	//MCUCR = 1<<ISC01 | 1<<ISC00; 	
	//sei();
    
    while (1)
    {
        do  
        {  
            KEY_PRT &= 0x0F;           //ground all rows of keyboard at once  
            colloc = (KEY_PIN & 0x0F);      //read the columns of keyboard  
        } while (colloc != 0x0F);     //keep checking until all keys released  
        do  
        {  
            do  
            {  
            delay_ms (5);      //call the delay  
            colloc =(KEY_PIN&0x0F); // check if any key is pressed   
            } while (colloc == 0x0F); //continuous checking for key press 
    				
            delay_ms (5);      //call the delay for debounce operation  
            colloc = (KEY_PIN & 0x0F);  //read the columns  
        } while (colloc == 0x0F);   // wait for the key press  
        while (1)  
        {  
            KEY_PRT = 0Xef;     //ground row 0 of keyboard  
            colloc = (KEY_PIN & 0x0F);//read  a columns  
            if (colloc != 0x0F)     //column is detected  
            {  
            rowlock =0;         //save the row location  
            break;              //exit while loop  
            }  
            KEY_PRT = 0xDF;     //ground row 1 of keyboard   
            colloc = (KEY_PIN & 0x0F);//read a columns  
            if (colloc != 0x0F)  
            {  
            rowlock =1;          //save the row location   
            break;  
            }  
            KEY_PRT = 0XBF;     //ground row 2 of keyboard  
            colloc = (KEY_PIN & 0x0F);//read a columns  
            if (colloc != 0x0F)     //column is detected  
            {  
            rowlock =2;         //save the row location  
            break;              //exit while loop  
            }  
            KEY_PRT = 0X7F;     //ground row 3 of keyboard  
            colloc = (KEY_PIN & 0x0F);//read a columns  
            rowlock = 3; //save the row location   
            break;  
        }
            
        if (colloc == 0x0E) // col 1
        {
            if (flag_ring)
            {
                switch(rowlock)
                {
                    case 0:
                        {
                            lcdCommand(0x01);
                            lcdCommand(0x80);  
                            lcd_gotoxy(1,1);
                            lcd_print(keypad[rowlock][0]);
                            PORTB=(1<<PORTB3); 
                            delay_ms(500);
                            PORTB=(0<<PORTB3); 
                            break;
                        }
                    case 1:
                        { 
                            lcdCommand(0x01);
                            lcdCommand(0x80);  
                            lcd_gotoxy(1,1);  
                            lcd_print(keypad[rowlock][0]);                        
                            PORTB=(1<<PORTB6); 
                            delay_ms(500);
                            PORTB=(0<<PORTB6);
                            break;
                        }
                    case 2:
                        { 
                            lcdCommand(0x01);
                            lcdCommand(0x80);  
                            lcd_gotoxy(1,1);  
                            lcd_print(keypad[rowlock][0]);                        
                            PORTD=(1<<PORTD6); 
                            delay_ms(500);
                            PORTD=(0<<PORTD6);
                            break;
                        }
                    case 3:
                        { 
                            lcdCommand(0x01);
                            lcdCommand(0x80);  
                            lcd_gotoxy(1,1);  
                            lcd_print("open");
                                  
                            PORTD=(1<<PORTD4);
                            delay_ms(500);                                
                            PORTD=(0<<PORTD4);
                            flag_ring = 0;
                            lcdCommand(0x01);
                            lcdCommand(0x80);  
                            lcd_gotoxy(1,1);
                            break;
                        }
                    
                }
            }
        }  
        		 
        else if (colloc == 0x0D)  // col 2
        {
            if (flag_ring)
            {
                switch(rowlock)
                {
                    case 0:
                        {
                            lcdCommand(0x01);
                            lcdCommand(0x80);  
                            lcd_gotoxy(1,1);
                            lcd_print(keypad[rowlock][1]);
                            PORTB=(1<<PORTB4); 
                            delay_ms(500);
                            PORTB=(0<<PORTB4);
                            break;                     
                        }
                    case 1:
                        { 
                            lcdCommand(0x01);
                            lcdCommand(0x80);  
                            lcd_gotoxy(1,1);  
                            lcd_print(keypad[rowlock][1]);                        
                            PORTB=(1<<PORTB7); 
                            delay_ms(500);
                            PORTB=(0<<PORTB7);
                            break;
                        }
                    case 2:
                        { 
                            lcdCommand(0x01);
                            lcdCommand(0x80);  
                            lcd_gotoxy(1,1);  
                            lcd_print(keypad[rowlock][1]);                        
                            PORTD=(1<<PORTD7); 
                            delay_ms(500);
                            PORTD=(0<<PORTD7);
                        
                            break;
                        }
                    
                }
            }	   
        }  
        else if (colloc == 0x0B)  //col 3
        {
            if(rowlock == 3)                     \\ guest
            {
                lcd_print("Ring which floor?");
                flag_ring = 1;                
            }
            else
            {
                if (flag_ring)
                {
                    switch(rowlock)
                    {
                        case 0:
                            {
                                lcdCommand(0x01);
                                lcdCommand(0x80);  
                                lcd_gotoxy(1,1);
                                lcd_print(keypad[rowlock][2]);
                                PORTB=(1<<PORTB5); 
                                delay_ms(500);
                                PORTB=(0<<PORTB5); 
                            
                                break;
                            }
                        case 1:
                            { 
                                lcdCommand(0x01);
                                lcdCommand(0x80);  
                                lcd_gotoxy(1,1);  
                                lcd_print(keypad[rowlock][2]);                        
                                PORTD=(1<<PORTD5); 
                                delay_ms(500);
                                PORTD=(0<<PORTD5);
                            
                                break;
                            }
                    }
                }
            }
            
        			
        			   
        }  

    }
}
