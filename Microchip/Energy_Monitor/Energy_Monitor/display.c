/*
 * display.c
 *
 * Created: 1/10/2026 4:22:26 PM
 *  Author: ethan
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "display.h"

#define SH_CP (1 << PORTC3)
#define SH_DS (1 << PORTC4)
#define SH_ST (1 << PORTC5)

void transmitDigit(uint8_t num, uint8_t place);

// Number format is pre-processed into the seven-segment representation
uint8_t voltage[4] = {0, 0, 0, 0};
uint8_t current[4] = {0, 0, 0, 0};
uint8_t power[4] = {0, 0, 0, 0};

uint8_t cycles = 0;
uint8_t type = 0;

// Storage of seven-segment rep for certain digits
uint8_t DIGITS[10] = {
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

volatile uint8_t place = 0;

ISR (TIMER0_COMPA_vect) {
	// Display the required digit
	switch (type) {
		case(VOLTAGE):
			transmitDigit(voltage[place], place);
		break;
		case(CURRENT):
			transmitDigit(current[place], place);
		break;
		case(POWER):
			transmitDigit(power[place], place);
		break;
	}
	
	// Show the next digit along next
	place++;
	if (place == 4) {place = 0;}
	
	// Move to different display if 1s passed
	cycles++;
	if (cycles == 200) {
		// Reset LEDs
		PORTB &= 0b11110000;
		cycles = 0;
		type++;
		if (type == 3) {type = 0;};
		PORTB |= (1 << type);
	}
}

void init_display(void) {
	// Initialise display pins
	DDRC |= (1 << DDC3);
	DDRC |= (1 << DDC4);
	DDRC |= (1 << DDC5);
	
	DDRD |= (1 << DDD4);
	DDRD |= (1 << DDD5);
	DDRD |= (1 << DDD6);
	DDRD |= (1 << DDD7);
	
	// Timer setup
	TCCR0A &= ~(1 << WGM00);
	TCCR0A |= (1 << WGM01);
	TCCR0B &= ~(1 << WGM02);
	
	// Set to reset every ~5ms (4.992) (scaled to /64)
	TCCR0B |= (1 << CS00);
	TCCR0B |= (1 << CS01);
	TCCR0B &= ~(1 << CS02);
	OCR0A |= 156;
	
	TIMSK0 |= (1 << OCIE0A);
	sei();
	
	update_value(1234, VOLTAGE);
	update_value(5678, CURRENT);
	update_value(9012, POWER);
	
	// Test LEDS
	DDRB |= (0b00001111);
}

void update_value(uint16_t num, uint8_t type) {
	// Split 4 digit number into the separate digits
	uint8_t digit0 = num%10;
	num = num/10;
	uint8_t digit1 = num%10;
	num = num/10;
	uint8_t digit2 = num%10;
	num = num/10;
	uint8_t digit3 = num%10;
	
	// Store digits as their seven-segment display version
	if (type == VOLTAGE) {
		voltage[0] = DIGITS[digit0];
		voltage[1] = DIGITS[digit1];
		voltage[2] = DIGITS[digit2];
		voltage[3] = DIGITS[digit3];
	} else if (type == CURRENT) {
		current[0] = DIGITS[digit0];
		current[1] = DIGITS[digit1];
		current[2] = DIGITS[digit2];
		current[3] = DIGITS[digit3];
	} else if (type == POWER) {
		power[0] = DIGITS[digit0];
		power[1] = DIGITS[digit1];
		power[2] = DIGITS[digit2];
		power[3] = DIGITS[digit3];
	}
}

void transmitDigit(uint8_t display, uint8_t place) {
	// Reset pins
	PORTD |= 0b11110000;
	// Set new pin
	PORTD &= ~(1 << (7 - place));
	
	PORTC &= ~SH_CP;
	PORTC &= ~SH_ST;
	
	for (int8_t i = 7; i >= 0; i--) {
		if (display & (1 << i)) {
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
}*/