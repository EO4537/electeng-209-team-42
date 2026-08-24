/*
 * UART Implementation.c
 *
 * Created: 24/08/2026 9:56:49 pm
 * Author : Aditya Thapliyal 
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define F_CPU 2000000UL
#include <util/delay.h>
#include "UART0.h"

// Macros
#define BAUD_RATE 9600UL
#define UBRR_VALUE (F_CPU / (16 * BAUD_RATE) -1)

// Values 
#define RMSVoltage 14.5
#define PeakCurrent 125
#define Power 1.60

 int main (void)
 {
	 uart_init(UBRR_VALUE);  // Initialize UART0
	 
	 char string_buffer[30];
	 
	 while(1)
	 {
		 // RMS Voltage 
		 uint16_t voltage = (uint16_t) (RMSVoltage * 10 + 0.5); //Scale by 10 to remove decimal point
		 uint8_t voltageTens = (voltage / 100) + 48;
		 uint8_t voltageUnits = ((voltage / 10) % 10 ) + 48;
		 uint8_t voltDecimal = (voltage % 10) + 48;
		 
		 sprintf(string_buffer , "RMS Voltage is: ");
		 uart_transmit_string(string_buffer);
		 
		 uart_transmit(voltageTens);  // Transmit Tens digit
		 uart_transmit(voltageUnits);   // Transmit Units digit
		 uart_transmit(46); // Transmit decimal point (.) 
		 uart_transmit(voltDecimal);    // Transmits Decimal digit
		 uart_transmit(13);             // Transmit new line 
		 uart_transmit(10);
		 
		 
		 // Peak Current
		 sprintf(string_buffer , "Peak Current is: %d" ,PeakCurrent);
		 uart_transmit_string(string_buffer);
		 
		 uart_transmit(13);  // Creating new line 
		 uart_transmit(10);  // Creating new line 
		 
		 // Power 
		 uint16_t power = (uint16_t) (Power * 100 + 0.5); //Scale by 100 to remove decimal point
		 uint8_t powerUnits = (power / 100) + 48; // Units digit 
		 uint8_t powerTens = ((power / 10) % 10 ) + 48; // First decimal digit
		 uint8_t powerOnes = (power % 10) + 48;   // Second Decimal digit
		 
		 sprintf(string_buffer , "Power is: ");
		 uart_transmit_string(string_buffer);
		 
		 uart_transmit(powerUnits);
		 uart_transmit(46); // Transmit decimal point (.)
		 uart_transmit(powerTens);
		 uart_transmit(powerOnes);
		 uart_transmit(13);             // Transmit new line
		 uart_transmit(10);
		  
		  
		  
		 _delay_ms(1000); // 1 second delay 
		 uart_transmit(13);    // Creating a different section 
		 uart_transmit(10);
		 
		 
	 }
	 
 }


