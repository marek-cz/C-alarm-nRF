/*
 * Program_nRF.c
 *
 * Created: 2022-04-16 10:25:34
 * Author : Marek
 */ 

#include <avr/io.h>
#include <avr/delay.h>

#include "GPIO/GPIO.h"


int main(void)
{
	
	gpioPin_t led2 = { .PORTx = &PORTD, .DDRx = &DDRD, .PINx = &PIND, .pinNumber = PD7 };
	gpioPin_t buzzer = { .PORTx = &PORTD, .DDRx = &DDRD, .PINx = &PIND, .pinNumber = PD4 };
	gpioPin_t alarmLed = { .PORTx = &PORTD, .DDRx = &DDRD, .PINx = &PIND, .pinNumber = PD5 };
	
	// Konfiguracja pinow jako wyjscia:
	setPinOutput(led2);
	setPinOutput(buzzer);
	setPinOutput(alarmLed);
	
	// stan poczatkowy
	setPin(led2);
	
	setPin(buzzer);
	_delay_ms(1000);
	clearPin(buzzer);
	
	for(uint8_t i = 0; i < 20; i++)
	{
		togglePin(alarmLed);
		_delay_ms(300);
	}
	
    /* Replace with your application code */
    while (1) 
    {
		_delay_ms(500);
		togglePin(led2);
    }
}

