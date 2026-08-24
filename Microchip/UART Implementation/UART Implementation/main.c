/*
 * UART Implementation.c
 *
 * Created: 24/08/2026 9:56:49 pm
 * Author : Aditya Thapliyal 
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include  <string.h>
#include <stdio.h>

 #define F_CPU 2000000UL
 #include <util/delay.h>
 #include "UART0.h"
 #include <util/delay.h>

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
		 // Peak Current
		 sprintf(string_buffer , "Peak Current is: %d" ,PeakCurrent);
		 for (int i =0 ; i <strlen(string_buffer); i++)
		 {
			 uart_transmit(string_buffer[i]); // Transmit  the formatted string
		 }
		 
		 uart_transmit(13);  // Creating new line 
		 uart_transmit(10);  // Creating new line 
		 
		 
		 _delay_ms(1000); // 1 second delay 
		 
		 
	 }
	 
 }


