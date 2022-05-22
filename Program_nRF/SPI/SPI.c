/*
 * SPI.c
 *
 * Created: 2022-05-22 14:07:33
 *  Author: Marek
 */ 

#include <avr/io.h>
#include "SPI.h"

void spiInit(void)
{
	//TODO
}

static void spiWriteByte( uint8_t data )
{
	//TODO
}

static uint8_t spiReadByte( void )
{
	//TODO
}

void spiWrite( uint8_t data, gpioPin_t nSS )
{
	//TODO
	clearPin(nSS);
	// write to reg
	setPin(nSS);
}

uint8_t spiRead( gpioPin_t nSS )
{
	//TODO
}
