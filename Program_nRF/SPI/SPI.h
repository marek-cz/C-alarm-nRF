/*
 * SPI.h
 *
 * Created: 2022-05-22 14:06:18
 *  Author: Marek
 */ 


#ifndef SPI_H_
#define SPI_H_

#include "../GPIO/GPIO.h"

void spiInit(void);
void spiWrite( uint8_t data, gpioPin_t nSS );
uint8_t spiRead( gpioPin_t nSS);

#endif /* SPI_H_ */