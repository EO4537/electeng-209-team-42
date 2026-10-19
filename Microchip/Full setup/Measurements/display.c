/*
 * display.c
 *
 * Created: 1/10/2026 4:22:26 PM
 *  Author: Ethan
 */
#include "display.h"

#include <avr/io.h>
#include <stdbool.h>

#include "parameters.h"

// Defining pins
#define SH_CP (1 << PORTC3)
#define SH_DS (1 << PORTC4)
#define SH_ST (1 << PORTC5)

// Internal functions not used by main.c
static void transmit_digit(uint8_t num, uint8_t place);

// Current data to display, stored as each digits seven-segment representation
static uint8_t data[4][4] = {
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0}
};

// Storage of seven-segment rep for certain digits
static const uint8_t DIGITS[10] = {
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

static const uint8_t LABELS[4] = {
	0b00111110,
	0b01110111,
	0b01110011,
	0b01111001
};

// Variables to track what digit from
// what data should be displayed
static volatile uint8_t type = 0;
static volatile uint8_t place = 0;
static volatile bool displayLabel = true;

void display_cycle_digit(void) {
	
	// Display the required digit
	if (displayLabel) {
		transmit_digit(LABELS[type], place);
	} else {
		transmit_digit(data[type][place], place);
	}
	
	// Show the next digit along next
	place++;
	if (place == 4) {place = 0;}
}

void display_cycle_value(void) {
		
	// Reset LEDs
	PORTB &= 0b11110000;
	PORTB |= (1 << type);
	
	if (displayLabel) {
		displayLabel = false;
	} else {
		type++;
		if (type == 4) {type = 0;};
		displayLabel = true;
	}
}

void display_init(void) {
	// Initialise display pins
	DDRC |= (1 << DDC3);
	DDRC |= (1 << DDC4);
	DDRC |= (1 << DDC5);
	
	DDRD |= (1 << DDD4);
	DDRD |= (1 << DDD5);
	DDRD |= (1 << DDD6);
	DDRD |= (1 << DDD7);
	
	display_error(VOLTAGE);
	display_error(CURRENT);
	display_error(POWER);
	display_error(ENERGY);
	
	// Set display type LEDs
	DDRB |= (0b00001111);
}

void display_update_value(uint8_t *num, uint8_t decimal, uint8_t type) {
	// Stores given data as arrays in 7 segment rep
	for (uint8_t i = 0; i < 4; i++) {
		data[type][i] = DIGITS[num[i]];
	}
	data[type][decimal] |= 0b10000000;
}

void display_error(uint8_t type) {
	// Set a number to display an error
	data[type][0] = 0b01000000;
	data[type][1] = 0b01000000;
	data[type][2] = 0b01000000;
	data[type][3] = 0b01000000;
}

static void transmit_digit(uint8_t display, uint8_t place) {
	// Reset pins
	PORTD |= 0b11110000;
	// Set multiplexer to output correct digit
	PORTD &= ~(1 << (7 - place));
	
	// Reset clock pin
	PORTC &= ~SH_CP;
	// Reset register output
	PORTC &= ~SH_ST;
	
	for (int8_t i = 7; i >= 0; i--) {
		// Set data pin to appropriate output for
		// current bit
		if (display & (1 << i)) {
			PORTC |= SH_DS;
		} else {
			PORTC &= ~SH_DS;
		}
		
		// Shift bit into register
		PORTC |= SH_CP;
		PORTC &= ~SH_CP;
	}
	
	// Activate register output
	PORTC |= SH_ST;
	PORTC &= ~SH_ST;
}

// Turn off the 7 segment display
void display_blank(void) {
	PORTB &= 0b11110000;
	for (uint8_t i = 0; i < 4; i++) {
		transmit_digit(0b00000000, i);
	}
}