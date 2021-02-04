#include <avr/io.h>
#include <avr/interrupt.h>


uint8_t i =0;
int sinewave[27] = {0,20,40,60,80,100,120,140,160,180,200,220,245,255,245,220,200,180,160,140,120,100,80,60,40,20,0};


ISR(TIMER0_OVF_vect)
{
	OCR0 = sinewave[i];
	if (i > 27)
	i=0;
	else
	i++;
}

int main(void)
{
	
	
	DDRB=0xFF;
	TCCR0=0x6C;
	TIMSK=(1 << TOIE0);
	TCNT0 = 0;
	OCR0 = 128;
	sei();
	while (1);
	return 1;
}

