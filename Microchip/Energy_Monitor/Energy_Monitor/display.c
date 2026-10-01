/*
 * display.c
 *
 * Created: 1/10/2026 4:22:26 PM
 *  Author: ethan
 */ 
#include <avr/io.h>
#include "display.h"

#define SH_CP (1 << PORTC3)
#define SH_DS (1 << PORTC4)
#define SH_ST (1 << PORTC5)

// Number format is pre-processed into the seven-segment representation
uint8_t voltage[4] = {0, 0, 0, 0};
uint8_t current[4] = {0, 0, 0, 0};
uint8_t power[4] = {0, 0, 0, 0};

void init_display(void) {
	
}

void transmitDigit(uint8_t num, uint8_t place) {
	// Reset pins
	PORTD |= 0b11110000;
	// Set new pin
	PORTD &= ~(1 << (7 - place));
	
	PORTC &= ~SH_CP;
	PORTC &= ~SH_ST;
	
	for (int8_t i = 7; i >= 0; i--) {
		uint8_t transmitBit = digits[num] & (1 << i);
		if (transmitBit) {
			PORTC |= SH_DS;
			} else {
			PORTC &= ~SH_DS;
		}
		
		PORTC |= SH_CP;
		PORTC &= ~SH_CP;
	}
	PORTC |= SH_ST;
	PORTC &= ~SH_ST;
}

/*
 * GccApplication2.c
 *
 * Created: 28/09/2026 1:14:52 PM
 * Author : ethan
 

#define F_CPU 2000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define SH_CP (1 << PORTC3)
#define SH_DS (1 << PORTC4)
#define SH_ST (1 << PORTC5)

void transmitDigit(uint8_t num, uint8_t place);

uint16_t number = 0;
volatile uint16_t place = 0;

uint8_t digits[10] = {
	0b00111111,
	0b00000110,
	0b01011011,
	0b01001111,
	0b01100110,
	0b01101101,
	0b01111101,
	0b00000111,
	0b01111111,
	0b01101111
};

uint8_t number[4] {0, 0, 0, 0};

ISR (TIMER0_COMPA_vect) {
	uint16_t digit = (number/(uint16_t)pow(10, place))%10;
	transmitDigit(digit, place);
	place++;
	if (place == 4) {place = 0;}
}

int main(void)
{
	// Initialise display
	DDRC |= (1 << DDC3);
	DDRC |= (1 << DDC4);
	DDRC |= (1 << DDC5);
	
	DDRD |= (1 << DDD4);
	DDRD |= (1 << DDD5);
	DDRD |= (1 << DDD6);
	DDRD |= (1 << DDD7);
	
	// Timer setup
	TCCR0A |= (1 << WGM01);
	
	// Set to reset every ~10ms
	TCCR0B |= (1 << CS02);
	//OCR0A |= 77;
	OCR0A |= 50;
	
	TIMSK0 |= (1 << OCIE0A);
	sei();
	
	// Display 7
	
	
     Replace with your application code 
    while (1) 
    {
		_delay_ms(1000);
		number++;
		
		
    }
}

void transmitDigit(uint8_t num, uint8_t place) {
	// Reset pins
	PORTD |= 0b11110000;
	// Set new pin
	PORTD &= ~(1 << (7 - place));
	
	PORTC &= ~SH_CP;
	PORTC &= ~SH_ST;
	
	for (int8_t i = 7; i >= 0; i--) {
		uint8_t transmitBit = digits[num] & (1 << i);
		if (transmitBit) {
			PORTC |= SH_DS;
		} else {
			PORTC &= ~SH_DS;
		}
		
		PORTC |= SH_CP;
		PORTC &= ~SH_CP;
	}
	PORTC |= SH_ST;
	PORTC &= ~SH_ST;
}/*