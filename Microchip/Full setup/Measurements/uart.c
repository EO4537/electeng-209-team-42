/*
 * uart.c
 *
 * Created: 6/10/2026 5:11:19 pm
 *  Author: Ethan
 */ 
#include "uart.h"
#include <string.h>
#include <stdint.h>

#define F_CPU 2000000UL
#define UBRR 9600

// ASCII Data of each stored value
static uint8_t uartData[4][5] = {
	{48, 48, 48, 48, 48},
	{48, 48, 48, 48, 48},
	{48, 48, 48, 48, 48},
	{48, 48, 48, 48, 48}
};

// Header to print above data
static const char header[] = "Voltage - Current - Power   - Energy";

// Local functions
static void uart_transmit_char(uint8_t character);
static void transmit_header(void);

// Tracker for how many times data has been printed
static uint8_t linesPrinted = 0;

void uart_init(void){
	UBRR0 = 12;
	UCSR0B |= 1 << TXEN0;  // Set TXEN0 bit to 1 ( enable transmit )
	UCSR0C |= ( 1 << UCSZ01) | (1 << UCSZ00) ;  // character size set to 8 bits
	
	transmit_header();
}

void uart_update_value(uint8_t *num, uint8_t decimal, uint8_t type){
	// Store each digit of the number as it's ascii value
	// Insert decimal point where specified
	uint8_t digitPlace = 3;
	for (int8_t charPlace = 4; charPlace >= 0; charPlace--){
		uartData[type][charPlace] = num[digitPlace] + 48;
		if (decimal == digitPlace){
			charPlace--;
			uartData[type][charPlace] = 46;
		}
		digitPlace--;
	}
}

static void uart_transmit_char(uint8_t character){
	while ((UCSR0A & (1 << UDRE0)) == 0){  // UDRE0 bit is checked. If its 0 wait for it to become 1 before transmitting the data
	}
	UDR0 = character;
}

static void transmit_header(void){
	uart_transmit_char(13);  // Creating new line
	uart_transmit_char(10);
	
	// Loop through header string and print
	for (int i =0 ; i <strlen(header); i++){
		uart_transmit_char(header[i]);
	}
	
	uart_transmit_char(13);  // Creating new line
	uart_transmit_char(10);
}

void uart_transmit(void){
	if (linesPrinted == 10) {
		transmit_header();
		linesPrinted = 0;
	}
	
	// Loop through each data type
	for (int i = 0; i < 4; i++){
		// Print each digit (and decimal)
		for (int j = 4; j >= 0; j--){
			uart_transmit_char(uartData[i][j]);
		}
		
		// Print spacing between numbers
		for (int j = 0; j < 5; j++){
			uart_transmit_char(32);	
		}
	}
	
	uart_transmit_char(13);  // Creating new line
	uart_transmit_char(10);
	
	linesPrinted++;
}