/*
 * uart.h
 *
 * Created: 6/10/2026 5:11:32 pm
 *  Author: Ethan
 */ 


#ifndef UART_H_
#define UART_H_

#include "util.h"

void uart_init(void);

void uart_update_value(uint8_t *num, uint8_t decimal, uint8_t type);

void uart_transmit(void);

#endif /* UART_H_ */