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
#define SPI_SPCR_SPE PB6
#define SPI_CLOCK_POLARITY (1<<3)
#define SPI_CLOCK_PHASE (1<<2)
#define SPI_DATA_ORDER (1<<5)
#define SPI_INTER_ENABLED 1<<7

//######################################################################

void spiInit(void);
void spiWrite( uint8_t* data, gpioPin_t nSS, int N );
void spiRead(  uint8_t* data, gpioPin_t nSS,int N );
uint8_t spiTransreceive( uint8_t data);

#endif /* SPI_H_ */