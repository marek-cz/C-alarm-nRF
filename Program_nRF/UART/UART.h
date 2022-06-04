/*
 * UART.h
 *
 * Created: 2022-06-04 19:14:16
 *  Author: Marek
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>

void uartInit(void);
void uartSendChar(uint8_t c);
void uartSendString(const char* str);
void uartSendHex(uint8_t c);


#endif /* UART_H_ */