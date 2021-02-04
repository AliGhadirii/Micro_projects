#include <avr/io.h>
#include <math.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

long i = 50;
long c = 0;
int state = 1;
long RR = 0;

#define KEY_PRT PORTC   //initializing keyboard PORT
#define KEY_DDR DDRC  //initializing keyboard DDR
#define KEY_PIN PINC        //initializing keyboard PIN


unsigned char * keypad [4][3] = {"1", "2", "3",
	"4", "5", "6",
	"7", "8", "9",
"*", "0", "#"};

unsigned char colloc, rowlock;
int counter = 0;
//char rrtemp[10]="";

ISR(TIMER0_OVF_vect)
{	PORTB = i;
	switch(state)
	{
		case 1:
		{
			i+= (30/(30*RR/857));
			if(i==80)
			{
				state = 2;
				c = 0;
			}
			break;
		}
		case 2:
		{
			c++;
			if(c==(30*RR/857)) //count to 30 ,each 1 msec
			state = 3;
			break;
		}
		case 3:
		{
			i-= (30/(30*RR/857));
			if(i==50)
			{
				state = 4;
				c = 0;
			}
			break;
		}
		case 4:
		{
			c++;
			if(c==(70*RR/857)) //count to 70 ,each 1 msec
			state = 5;
			break;
		}
		case 5:
		{
			i-= (15 / (5*RR/857));
			if(i==35)
			{
				state = 6;
				c = 0;
			}
			break;
		}
		case 6:
		{
			i+= (180 / (30*RR/857));
			if(i==215)
			{
				state = 7;
				c = 0;
			}
			break;
		}
		case 7:
		{
			i-= (192 / (32*RR/857));
			if(i==23)
			{
				state = 8;
				c = 0;
			}
			break;
		}
		case 8:
		{
			i += (27 / (9*RR/857));
			if(i==50)
			{
				state = 9;
				c = 0;
			}
			break;
		}
		case 9:
		{
			c++;
			if(c==(89*RR/857)) //count to 89 ,each 1 msec
			state = 10;
			break;
		}
		case 10:
		{
			i+= (55 / (55*RR/857));
			if(i==105)
			{
				state = 11;
				c = 0;
			}
			break;
		}
		case 11:
		{
			c++;
			if(c==(55*RR/857)) //count to 89 ,each 1 msec
			state = 12;
			break;
		}
		case 12:
		{
			i-= (55 / (55*RR/857));
			if(i==50)
			{
				state = 13;
				c = 0;
			}
			break;
		}
		case 13:
		{
			c++;
			if(c==(367*RR/857)) //count to 89 ,each 1 msec
			state = 1;
			break;
		}
	}

}

ISR (INT0_vect)
{
		
		
	do
	{
		do
		{
			_delay_ms (5);      //call the delay
			colloc =(KEY_PIN&0x0F); // check if any key is pressed
		} while (colloc == 0x0F); //continuous checking for key press
			
		_delay_ms (5);      //call the delay for debounce operation
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
		switch (counter)
		{
			case 0:
				RR += atoi(keypad[rowlock][0]) * 100;
				counter ++;
				break;
			case 1:
				RR += atoi(keypad[rowlock][0]) * 10;
				counter ++;
				break;
			case 2:
				RR += atoi(keypad[rowlock][0]);
				counter = 0;
				DDRB = 0xFF;
				TCCR0 = 0x02;
				TIMSK = (1 << TOIE0);
				TCNT0 = 0;
				PORTD = (1 << PORTD7);
				sei();
				break;
		}
			
	}
		
	else if (colloc == 0x0D)  // col 2
	{
		switch (counter)
		{
			case 0:
			RR += atoi(keypad[rowlock][1]) * 100;
			counter ++;
			break;
			case 1:
			RR += atoi(keypad[rowlock][1]) * 10;
			counter ++;
			break;
			case 2:
			RR += atoi(keypad[rowlock][1]);
			counter = 0;
			DDRB = 0xFF;
			TCCR0 = 0x02;
			TIMSK = (1 << TOIE0);
			TCNT0 = 0;
			PORTD = (1 << PORTD7);
			sei();
			break;
		}
			
	}
		
	else if (colloc == 0x0B)  //col 3
	{
		switch (counter)
		{
			case 0:
			RR += atoi(keypad[rowlock][2]) * 100;
			counter ++;
			break;
			case 1:
			RR += atoi(keypad[rowlock][2]) * 10;
			counter ++;
			break;
			case 2:
			RR += atoi(keypad[rowlock][2]);
			counter = 0;
			DDRB = 0xFF;
			TCCR0 = 0x02;
			TIMSK = (1 << TOIE0);
			TCNT0 = 0;
			PORTD = (1 << PORTD7);
			sei();
			break;
		}
	}
	KEY_DDR = 0xF0;
	KEY_PRT = 0x0F;
	sei();

}

int main(void)
{
	KEY_DDR = 0xF0;
	KEY_PRT = 0x0F;
	GICR = 1<<INT0;
	MCUCR = 0x02;
	DDRD = 0x80;
	PORTD = 0x7F;
	
	
	sei();
	while(1)
	{
		do
		{
			KEY_PRT &= 0x0F;           //ground all rows of keyboard at once
			colloc = (KEY_PIN & 0x0F);      //read the columns of keyboard
		} while (colloc != 0x0F);     //keep checking until all keys released	
	}
	return 1;
}


