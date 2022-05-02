/*
 * Program_nRF.c
 *
 * Created: 2022-04-16 10:25:34
 * Author : Marek
 */ 

#include <avr/io.h>
#include <avr/delay.h>

typedef struct  
{
	uint8_t PORTx;
	uint8_t DDRx;
	uint8_t PINx;
	uint8_t pinNumber;
}gpioPin_t;

int main(void)
{
	/*
	DDRD	|= (1<<7);
	PORTD	|= (1<<7);
	*/
	gpioPin_t led2 = { PORTD, DDRD, PIND, 7 };
	led2.DDRx |= (1 << led2.pinNumber);
	led2.PORTx |= (1 << led2.pinNumber);
	
    /* Replace with your application code */
    while (1) 
    {
		_delay_ms(500);
		led2.PORTx ^= (1 << led2.pinNumber);
    }
}

