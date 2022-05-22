/*
 * SPI.h
 *
 * Created: 2022-05-22 14:06:18
 *  Author: Marek
 */ 


#ifndef SPI_H_
#define SPI_H_

void spiInit(void);
void spiWrite( uint8_t data );
uint8_t spiRead(void);

#endif /* SPI_H_ */