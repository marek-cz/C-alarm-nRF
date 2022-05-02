/*
 * Program_nRF.c
 *
 * Created: 2022-04-16 10:25:34
 * Author : Marek
 */ 

#include <avr/io.h>
#include <avr/delay.h>


int main(void)
{
	
	DDRD	|= (1<<7);
	PORTD	|= (1<<7);
	
    /* Replace with your application code */
    while (1) 
    {
		_delay_ms(500);
		PORTD ^= (1<<7);
    }
}

