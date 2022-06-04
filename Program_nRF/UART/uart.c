/*
 * uart.c
 *
 * Created: 2022-06-04 14:29:24
 *  Author: Marek
 */ 

#include "UART.h"

#define UART_UCSRnA_TXC		(1<<6)
#define UART_UCSRnA_UDRE	(1<<5)

#define UART_UCSRnB_RXEN (1<<4)
#define UART_UCSRnB_TXEN (1<<3)

#define UART_UCSRnC_UPM1	(1<<5)
#define UART_UCSRnC_UPM0	(1<<4)
#define UART_UCSRnC_USBS	(1<<3)
#define UART_UCSRnC_UCSZ1	(1<<2)
#define UART_UCSRnC_UCSZ0	(1<<1)

#define USART_BAUDRATE 4800
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void uartInit(void)
{
	UBRR0H = (BAUD_PRESCALE >> 8);        //wpisanie starszego bajtu
	UBRR0L = BAUD_PRESCALE;             //wpisanie mlodszego bajtu
	
	UCSR0A = 0x00;
	UCSR0B = (UART_UCSRnB_RXEN)|(UART_UCSRnB_TXEN);
	UCSR0C = UART_UCSRnC_UCSZ1 | UART_UCSRnC_UCSZ0;
}

void uartSendChar(uint8_t c)
{
	UDR0 = c;
	while( ! (UCSR0A & UART_UCSRnA_UDRE) );
}

void uartSendString(const char* str)
{
	while( *str )
	{
		uartSendChar(*str);
		str++;
	}
}

static uint8_t bitsToHex(uint8_t bits)
{
	
	bits &= 0x0F;
	
	switch(bits)
	{
		case 0	:
		case 1	:
		case 2	:
		case 3	:
		case 4 	:
		case 5	:
		case 6	:
		case 7	:
		case 8	:
		case 9 	:	return '0' + bits;
					break;
		case 10 :
		case 11 :
		case 12 :
		case 13 :
		case 14 :
		case 15 :	return 'A' + (bits-10);
					break;
	}
	
	return 'X';
	
}

void uartSendHex(uint8_t c)
{
	uint8_t high4Bits = bitsToHex((c >> 4)&0x0F);
	uint8_t low4Bits = bitsToHex(c & 0x0F);
	
	uartSendChar('0');
	uartSendChar('x');
	uartSendChar(high4Bits);
	uartSendChar(low4Bits);
}