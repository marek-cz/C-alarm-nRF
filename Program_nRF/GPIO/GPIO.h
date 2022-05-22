/*
 * GPIO.h
 *
 * Created: 2022-05-22 14:55:21
 *  Author: Marek
 */ 


#ifndef GPIO_H_
#define GPIO_H_


typedef struct
{
	volatile uint8_t * PORTx;
	volatile uint8_t * DDRx;
	volatile uint8_t * PINx;
	uint8_t pinNumber;
}gpioPin_t;


void togglePin( gpioPin_t pin );
void setPin( gpioPin_t pin );
void clearPin(gpioPin_t pin);
void setPinOutput( gpioPin_t pin );


#endif /* GPIO_H_ */