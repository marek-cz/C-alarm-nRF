/*
 * Program_nRF.c
 *
 * Created: 2022-04-16 10:25:34
 * Author : Marek
 */ 

#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

#include "GPIO/GPIO.h"

#define NRF_TX
#ifndef NRF_TX
	#define NRF_RX
#endif

volatile uint8_t alarmFlag = 0;
volatile uint8_t nrfDataInt = 0;

ISR(INT1_vect)
{
	// rising edge interrupt
	alarmFlag = 1;
}

// Obs³uga przerwania PCINT - RX
ISR(PCINT0_vect)
{
	// falling edge interrupt
	nrfDataInt = 1;
}


int main(void)
{
	
	gpioPin_t led1 = { .PORTx = &PORTD, .DDRx = &DDRD, .PINx = &PIND, .pinNumber = PD6 };
	gpioPin_t led2 = { .PORTx = &PORTD, .DDRx = &DDRD, .PINx = &PIND, .pinNumber = PD7 };
	gpioPin_t buzzer = { .PORTx = &PORTD, .DDRx = &DDRD, .PINx = &PIND, .pinNumber = PD4 };
	gpioPin_t alarmLed = { .PORTx = &PORTD, .DDRx = &DDRD, .PINx = &PIND, .pinNumber = PD5 };
	gpioPin_t extInt0 = { .PORTx = &PORTB, .DDRx = &DDRB, .PINx = &PINB, .pinNumber = PB0 };
	gpioPin_t extInt1 = { .PORTx = &PORTD, .DDRx = &DDRD, .PINx = &PIND, .pinNumber = PD3 };
	// linie SPI - nRF
	gpioPin_t CE = { .PORTx = &PORTB, .DDRx = &DDRB, .PINx = &PINB, .pinNumber = PB1 };
	gpioPin_t nSS = { .PORTx = &PORTB, .DDRx = &DDRB, .PINx = &PINB, .pinNumber = PB2 };
	gpioPin_t mosi = { .PORTx = &PORTB, .DDRx = &DDRB, .PINx = &PINB, .pinNumber = PB3 };
	gpioPin_t miso = { .PORTx = &PORTB, .DDRx = &DDRB, .PINx = &PINB, .pinNumber = PB4 };
	gpioPin_t sclk = { .PORTx = &PORTB, .DDRx = &DDRB, .PINx = &PINB, .pinNumber = PB5 };	
	
	// Konfiguracja pinow jako wyjscia:
	setPinOutput(led1);
	setPinOutput(led2);
	setPinOutput(buzzer);
	setPinOutput(alarmLed);
	setPinOutput(sclk);
	setPinOutput(mosi);
	setPinOutput(nSS);

#ifdef NRF_TX
	
	// konfiguracja przerwan
	
	sei();   // odblokowujemy przyjmowanie przerwañ
	
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
		if( 1 == alarmFlag )
		{
			// todo
			alarmFlag = 0;	
			togglePin(led2);
		}
    }
#endif

#ifdef NRF_RX
	// konfiguracja przerwan

	sei();   // odblokowujemy przyjmowanie przerwañ

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
		if( 1 == nrfDataInt )
		{
			// todo
			nrfDataInt = 0;
			togglePin(led2);
		}
	}
#endif

}

