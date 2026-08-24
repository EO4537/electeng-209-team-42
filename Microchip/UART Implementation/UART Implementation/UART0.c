#include "UART0.h"

void uart_init(uint16_t ubrr) {
	UBRR0H = (ubrr >> 8 );
	UBRR0L = ubrr;
	UCSR0A = 0b00000000;   //  It contains read only flag bits controlled by hardware
	UCSR0B |= 1 << TXEN0;  // Set TXEN0 bit to 1 ( enable transmit )
	UCSR0C |= ( 1 << UCSZ01) | (1 << UCSZ00) ;  // character size set to 8 bits

}

void uart_transmit(uint8_t data){
	while ((UCSR0A & (1 << UDRE0)) == 0){  // UDRE0 bit is checked. If its 0 wait for it to become 1 before transmitting the data
	}
	
	UDR0 = data;  // Put the data to be sent into the UDR0 register
}

void uart_transmit_string(char* string_buffer){
	 for (int i =0 ; i <strlen(string_buffer); i++)
	 {
		 uart_transmit(string_buffer[i]); // Transmit  the formatted RMS voltage string
	 }
}