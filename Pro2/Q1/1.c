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

int flag_ring = 0;      
int first_num_flag = 0;
int first_num;   
char passwords_array [8][5] = {"1111", "2222", "3333", "4444", "5555", "6666", "7777", "8888"};
char str_password [5] = "";
 

 
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

ISR (2)
{
                
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
                            lcd_print("Door Opened");
                                                  
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

            else if(first_num_flag == 0){// the floor num that wants to enter the password
                lcd_print(keypad[rowlock][0]);
                lcd_print(" Enter password : ");
                first_num = atoi(keypad[rowlock][0]);
                first_num_flag ++;
                delay_ms(100);
                lcdCommand(0x01);
                lcdCommand(0x80);  
                lcd_gotoxy(1,1);
            }
            else {
                lcd_print(keypad[rowlock][0]);
                if (first_num_flag == 4){ //password has completed 
                    strcat(str_password, keypad[rowlock][0]);
                       
                    if(strcmp (str_password, passwords_array[first_num - 1])==0)
                    {
                         lcdCommand(0x01);
                         lcdCommand(0x80);  
                         lcd_gotoxy(1,1);
                         lcd_print("Correct Password");
                         PORTD=(1<<PORTD4);
                         delay_ms(500);                                
                         PORTD=(0<<PORTD4);
                         lcdCommand(0x01);
                         lcdCommand(0x80);  
                         lcd_gotoxy(1,1);
                         first_num_flag = 0;
                         strcpy(str_password, "");
                             
                    } 
                    else
                    {
                         lcdCommand(0x01);
                         lcdCommand(0x80);  
                         lcd_gotoxy(1,1);
                         lcd_print("Password wrong");
                         delay_ms(500);
                         lcdCommand(0x01);
                         lcdCommand(0x80);  
                         lcd_gotoxy(1,1);
                         first_num_flag = 0;
                         strcpy(str_password, "");
                    } 
                    
                        
                }
                else
                {    

                     //lcd_print(keypad[rowlock][0]);
                     strcat(str_password, keypad[rowlock][0]);
                     first_num_flag ++;  
                }
                
            }       
    }  
                 
    else if (colloc == 0x0D)  // col 2
    {
        //lcd_print(keypad[rowlock][1]);   
        if(rowlock == 0 || rowlock == 1 || rowlock == 2){
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
            
            
            else if(first_num_flag == 0){// the floor num that wants to enter the password
                lcd_print(keypad[rowlock][1]); 
                lcd_print(" Enter password : ");
                first_num = atoi(keypad[rowlock][1]);
                first_num_flag ++;
                delay_ms(100);
                lcdCommand(0x01);
                lcdCommand(0x80);  
                lcd_gotoxy(1,1);
            }
            else {
                lcd_print(keypad[rowlock][1]);
                if (first_num_flag == 4){ //password has completed 
                    strcat(str_password, keypad[rowlock][1]);
                       
                    if(strcmp (str_password, passwords_array[first_num - 1])==0)
                    {
                         lcdCommand(0x01);
                         lcdCommand(0x80);  
                         lcd_gotoxy(1,1);
                         lcd_print("Correct Password");
                         PORTD=(1<<PORTD4);
                         delay_ms(500);                                
                         PORTD=(0<<PORTD4);
                         lcdCommand(0x01);
                         lcdCommand(0x80);  
                         lcd_gotoxy(1,1);
                         first_num_flag = 0;
                         strcpy(str_password, "");
                             
                    } 
                    else
                    {
                         lcdCommand(0x01);
                         lcdCommand(0x80);  
                         lcd_gotoxy(1,1);
                         lcd_print("Password wrong");
                         delay_ms(500);
                         lcdCommand(0x01);
                         lcdCommand(0x80);  
                         lcd_gotoxy(1,1);
                         first_num_flag = 0;
                         strcpy(str_password, "");
                    } 
                    
                        
                }
                else
                {    

                     //lcd_print(keypad[rowlock][0]);
                     strcat(str_password, keypad[rowlock][1]);
                     first_num_flag ++;  
                }
                
            }
        }
               
    }  
        
    else if (colloc == 0x0B)  //col 3
    {
        //lcd_print(keypad[rowlock][2]);   
        if(rowlock == 0 || rowlock == 1 || rowlock == 2){
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
                        // where is case 2
                }
            }
            
            
            else if(first_num_flag == 0){// the floor num that wants to enter the password
                lcd_print(keypad[rowlock][2]);
                lcd_print(" Enter password : ");
                first_num = atoi(keypad[rowlock][2]);
                first_num_flag ++;
                delay_ms(100);
                lcdCommand(0x01);
                lcdCommand(0x80);  
                lcd_gotoxy(1,1);
            }
            else {
                lcd_print(keypad[rowlock][2]);
                if (first_num_flag == 4){ //password has completed 
                    strcat(str_password, keypad[rowlock][2]);
                       
                    if(strcmp (str_password, passwords_array[first_num - 1])==0)
                    {
                         lcdCommand(0x01);
                         lcdCommand(0x80);  
                         lcd_gotoxy(1,1);
                         lcd_print("Correct Password");
                         PORTD=(1<<PORTD4);
                         delay_ms(500);                                
                         PORTD=(0<<PORTD4);
                         lcdCommand(0x01);
                         lcdCommand(0x80);  
                         lcd_gotoxy(1,1);
                         first_num_flag = 0;
                         strcpy(str_password, "");
                             
                    } 
                    else
                    {
                         lcdCommand(0x01);
                         lcdCommand(0x80);  
                         lcd_gotoxy(1,1);
                         lcd_print("Password wrong");
                         delay_ms(500);
                         lcdCommand(0x01);
                         lcdCommand(0x80);  
                         lcd_gotoxy(1,1);
                         first_num_flag = 0;
                         strcpy(str_password, "");
                    } 
                    
                        
                }
                else
                {    

                     //lcd_print(keypad[rowlock][0]);
                     strcat(str_password, keypad[rowlock][2]);
                     first_num_flag ++;  
                }
                
            }
        } 
         else if(rowlock == 3) // # has been pushed => guest
         {  
            lcd_print("Ring which floor?");
            flag_ring = 1;   
         }
            		   
    }  
    KEY_DDR = 0xF0;
    KEY_PRT = 0x0F;
    sei();
}


void main(void)
{
    // Declare your local variables here  
    
    
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
    KEY_PRT = 0x0F;

    // Port D initialization
    // Function: Bit7=Out Bit6=Out Bit5=Out Bit4=Out Bit3=In Bit2=In Bit1=In Bit0=In 
    DDRD=(1<<DDD7) | (1<<DDD6) | (1<<DDD5) | (1<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
    // State: Bit7=0 Bit6=0 Bit5=0 Bit4=0 Bit3=T Bit2=T Bit1=T Bit0=T 
    PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);


    lcd_init();
    lcd_gotoxy(1,1);
    
      
    GICR = 1<<INT0;		
	MCUCR = 0x02; 	
	sei();
    
    while (1)
    {
        do  
        {  
            KEY_PRT &= 0x0F;           //ground all rows of keyboard at once  
            colloc = (KEY_PIN & 0x0F);      //read the columns of keyboard  
        } while (colloc != 0x0F);     //keep checking until all keys released  
    }
}
