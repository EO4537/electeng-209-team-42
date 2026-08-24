/*  UART0.h 
*/

#ifndef  UART0_H
#define UART0_H

#include <avr/io.h>

void uart_init(uint16_t ubrr); // Initialize UART0 with the given UBRR value  

void uart_transmit(uint8_t data); // Transmit a single character over USART0

#endif