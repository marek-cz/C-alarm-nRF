/*
 * Program_nRF.c
 *
 * Created: 2022-04-16 10:25:34
 * Author : Marek
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "GPIO/GPIO.h"
#include "nRF/nRF.h"
#ifdef DEBUG
#include "UART/UART.h"
#endif

#define NRF_TX
#ifndef NRF_TX
	#define NRF_RX
#endif

volatile uint8_t alarmFlag = 0;
volatile uint8_t nrfDataInt = 0;

volatile uint8_t nrf24_rx_flag, nrf24_tx_flag, nrf24_mr_flag; // zadeklarowane w pliku nRF.c

ISR(INT1_vect)
{
	// rising edge interrupt
	alarmFlag = 1;
}

// Obs³uga przerwania PCINT - RX
ISR(PCINT0_vect)
{
	// falling edge interrupt
	if( 0 == (PORTB & PB0) )//falling edge
	{
		nrfDataInt = 1;
	}
}

static inline void buzzerPeak(gpioPin_t buzzer_pin)
{
	setPin(buzzer_pin);
	_delay_ms(1000);
	clearPin(buzzer_pin);
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
	// linie UART
	
	// Konfiguracja pinow jako wyjscia:
	setPinOutput(led1);
	setPinOutput(led2);
	setPinOutput(buzzer);
	setPinOutput(alarmLed);
	setPinOutput(sclk);
	setPinOutput(mosi);
	setPinOutput(nSS);
	setPinOutput(CE);
	// Konfiguracja pinow jako wejscia
	setPinInput(miso);
	setPinInput(extInt0);
	setPinInput(extInt1);
	// stan poczatkowy
	setPin(led1);
	setPin(led2);
	setPin(nSS);
	
#ifdef DEBUG
	uartInit();
	uartSendString("Hello world!\n\r");
#endif	
	

#ifdef NRF_TX
	// konfiguracja przerwan
	EICRA |= (1<<ISC11)|(1<<ISC10); //rising edge
	EIMSK|=(1<<INT1);
	sei();   // odblokowujemy przyjmowanie przerwañ
	
	// konfiguracja nRF
	nRF24_Init(nSS, CE);
	nRF24_SetRXAddress(0, (uint8_t*) "Nad");
	nRF24_SetTXAddress((uint8_t*)"Odb");
	nRF24_TX_Mode();
	// sygnalziacja zakonczenia konfiguracji
	buzzerPeak(buzzer);
	clearPin(led1);
	
    /* Replace with your application code */
    while (1) 
    {
		if( 1 == alarmFlag )
		{
			// todo
			alarmFlag = 0;
		}
		
		for( uint8_t i=0; i<10; i++)
		{
			nRF24_WriteTXPayload(&i);
			_delay_ms(1);
			nRF24_WaitTX();
			togglePin(led1);
			_delay_ms(1000);
		}
		
    }
#endif

#ifdef NRF_RX
	
	uint8_t readData;
	
	// konfiguracja przerwan
	PCICR |= (1<<PCIE0);
	PCMSK0 |= (1<<PCINT0);

	sei();   // odblokowujemy przyjmowanie przerwañ

	// konfiguracja nRF
	nRF24_Init(nSS, CE);
	nRF24_SetRXAddress(0, (uint8_t*) "Odb");
	nRF24_SetTXAddress((uint8_t*)"Nad");
	nRF24_RX_Mode();
	
	// sygnalziacja zakonczenia konfiguracji
	buzzerPeak(buzzer);

	/* Replace with your application code */
	while (1)
	{
		if( 1 == nrfDataInt )
		{
			// todo
			nrfDataInt = 0;
			togglePin(led1);
			buzzerPeak(buzzer);
		}
		if ( nRF24_RXAvailible() )
		{
			nRF24_ReadRXPaylaod(&readData);
			togglePin(led2);
		}
	}
#endif

}

