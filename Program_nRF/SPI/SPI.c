/*
 * SPI.c
 *
 * Created: 2022-05-22 14:07:33
 *  Author: P.Czekala
 */ 

#include <avr/io.h>
#include "SPI.h"

void spiInit(void) //SPI initialization
{
	//TODO
	SPI_DDR |= (1<<MOSI)|(1<<SCK)|(1<<SS);
	SET_SS;//high state on SS line
	//SPCR &= SPI_CLOCK_POLARITY; // clock polarity bit cleared
	//SPCR &= SPI_CLOCK_PHASE;// clock phase bit cleared
	SPCR |= SPI_DATA_ORDER; // the LSB of the data word is transmitted first.
	SPCR &= SPI_CLOCK_POLARITY & SPI_CLOCK_PHASE;// clock polarity and clock phase bits cleared
	SPCR |= SPI_DATA_ORDER | (1<<MSTR) | SPI_SPCR_SPE | (SPR0)| SPI_INTER_ENABLED;
	//SPCR |= SPI_SPCR_SPE;// SPI enable
}


uint8_t spiTransreceive(uint8_t data)
{
	//transmit and/or receive 1 byte of data
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
	
}
void spiWrite( uint8_t* data, gpioPin_t nSS,int N )// writing data to SPI
{
	//TODO
	clearPin(nSS);
	for (uint8_t it=0;it<N;++it)
	{
		spiTransreceive(data[it]);
	}
	setPin(nSS);
}

uint8_t spiRead( gpioPin_t nSS )// reading data from SPI
{
	
	clearPin(nSS);
	uint8_t data = spiTransreceive(0x00);
	setPin(nSS);
	return data;
	
}
