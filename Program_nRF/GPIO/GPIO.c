/*
 * GPIO.c
 *
 * Created: 2022-05-22 14:56:52
 *  Author: Marek
 */ 

#include <avr/io.h>
#include "GPIO.h"

void togglePin( gpioPin_t pin )
{
	//*(pin->PORTx) ^= ( 1 << pin->pinNumber );
	*(pin.PORTx) ^= (1 << pin.pinNumber );
}

void setPin( gpioPin_t pin )
{
	*(pin.PORTx) |= (1 << pin.pinNumber );
}

void clearPin(gpioPin_t pin)
{
	*(pin.PORTx) &= ~(1 << pin.pinNumber );
}

void setPinOutput( gpioPin_t pin )
{
	*(pin.DDRx) |= ( 1 << pin.pinNumber );
}