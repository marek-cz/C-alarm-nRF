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
	volatile uint8_t * PORTx;
	volatile uint8_t * DDRx;
	volatile uint8_t * PINx;
	uint8_t pinNumber;
}gpioPin_t;

void togglePin( gpioPin_t pin )
{
	//*(pin->PORTx) ^= ( 1 << pin->pinNumber );
	*(pin.PORTx) ^= (1 << pin.pinNumber );
}

int main(void)
{
	
	gpioPin_t led2 = { .PORTx = &PORTD, .DDRx = &DDRD, .PINx = &PIND, .pinNumber = PD7 };
	*(led2.DDRx)  |= (1 << led2.pinNumber);
	*(led2.PORTx) |= (1 << led2.pinNumber);
	
    /* Replace with your application code */
    while (1) 
    {
		_delay_ms(500);
		togglePin(led2);
    }
}

