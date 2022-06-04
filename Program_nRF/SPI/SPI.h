/*
 * SPI.h
 *
 * Created: 2022-05-22 14:06:18
 *  Author: P. Czekala
 */ 


#ifndef SPI_H_
#define SPI_H_

#include "../GPIO/GPIO.h"

// SPI I/O PINS
#define SPI_DDR DDRB
#define SPI_PORT PORTB
#define MOSI PB3
#define SCK PB5
#define SS PB2
// SPI nSS SET/CLEAR MACRO
#define SET_SS SPI_PORT |=(1<<SS)
#define CLR_SS SPI_PORT &=~(1<<SS)
// SPI CONFIG BITS
#define SPI_SPCR_SPR0	(1<<0)
#define SPI_SPCR_SPR1	(1<<1)
#define SPI_SPCR_CPHA	(1<<2)
#define SPI_SPCR_CPOL	(1<<3)
#define SPI_SPCR_MSTR	(1<<4)
#define SPI_SPCR_DORD	(1<<5)
#define SPI_SPCR_SPE	(1<<6)
#define SPI_SPCR_SPIE	(1<<7)

//######################################################################

void spiInit(void);
void spiWrite( uint8_t* data, gpioPin_t nSS, int N );
void spiRead(  uint8_t* data, gpioPin_t nSS,int N );
uint8_t spiTransreceive( uint8_t data);

#endif /* SPI_H_ */