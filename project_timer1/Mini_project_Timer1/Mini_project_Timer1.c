/*
 * Mini_project_Timer1.c
 *
 *  Created on: 13/12/2019
 *      Author: mostafa fadali
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


unsigned char sec1=0;
unsigned char sec2=0;

unsigned char min1=0;
unsigned char min2=0;

unsigned char hr1=0;
unsigned char hr2=0;

void inerrupt_func(void);
/*ISR(TIMER1_COMPA_vect);
ISR(INT0_vect) ;
ISR(INT1_vect);
ISR(INT2_vect);*/
/*-------------------------------------------------*/
int main( )
{
	/* i/p && o/p */
	DDRB &= ~(1<<PB2);
	PORTB |= (1<<PB2); //internal pull up
	DDRD &= ~(1<<PD2);
	PORTD |= (1<<PD2); //internal pull up
	DDRD &= ~(1<<PD3);
	SREG|=(1<<7); //I-bit for
	inerrupt_func();
	DDRC |= 0x0F;
	PORTC &= 0xF0;
	DDRA|=(1<<PA0)|(1<<PA1)|(1<<PA2)|(1<<PA3)|(1<<PA4)|(1<<PA5);//output
	PORTA=0;

	while(1)
	{
		/***************************Display************************************/
		PORTA= (1<<PA0);//enable PORTA
		PORTC= (PORTC & 0) | sec1;//Display

		_delay_ms(5);

		PORTA= (1<<PA1);
		PORTC = (PORTC & 0) | sec2;
		_delay_ms(5);

		PORTA= (1<<PA2);
		PORTC= (PORTC & 0) | min1;

		_delay_ms(5);
		PORTA= (1<<PA3);
		PORTC = (PORTC & 0) | min2;
		_delay_ms(5);
		PORTA= (1<<PA4);
		PORTC= (PORTC & 0) | hr1;
		_delay_ms(5);
		PORTA= (1<<PA5);
		PORTC = (PORTC & 0) | hr2;
		_delay_ms(5);
	}
	//return 0;
}


/*------------------func. interrupt-----------------------*/
void inerrupt_func(void)
{
	MCUCR|=(1<<ISC01)|(1<<ISC11)|(1<<ISC10);
	MCUCR &= ~(1<<ISC00);
	MCUCSR &= ~(1<<ISC2);

	GICR|=(1<<INT1)|(1<<INT0)|(1<<INT2);
	//TIMER
	TCCR1A=(1<<FOC1A);
	TCCR1B=(1<<WGM12)|(1<<CS12)|(1<<CS10);
	TCNT1 = 0;
	OCR1A=976;

	TIMSK=(1<<OCIE1A); //enable interrupt
}
/*---------------------------------------------*/
ISR(TIMER1_COMPA_vect)
{
	/*ISR of timer1*/

	sec1++;

	if(sec1==10)
	{
		sec1=0;
		sec2++;

		if(sec2==6)
		{
			sec2=0;
			min1++;
			if(min1==10)
			{
				min1=0;
				min2++;
				if(min2==6)
				{
					min2=0;
					hr1++;
					if(hr1==10)
					{
						hr1=0;
						hr2++;
						if(hr2==3)
						{
							hr2=0;


						}

					}

				}

			}
		}



	}

}

ISR(INT0_vect)/*Reset --> return all time to 0*/
{

	sec1=sec2=min1=min2=hr1=hr2=0;

}

ISR(INT1_vect) /*Pause --> no clock */

{
	TCCR1B=(1<<WGM12);

}

ISR(INT2_vect)
{
	TCCR1B=(1<<WGM12)|(1<<CS12)|(1<<CS10);
}
