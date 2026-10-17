/*
 * display.c
 *
 * Created: 1/10/2026 4:22:26 PM
 *  Author: Ethan
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "display.h"
#include "comms.h"

// Defining pins
#define SH_CP (1 << PORTC3)
#define SH_DS (1 << PORTC4)
#define SH_ST (1 << PORTC5)

// Internal functions not used by main.c
static void transmitDigit(uint8_t num, uint8_t place);
static void set_error(uint8_t type);

// Current data to display, stored as each digits seven-segment representation
uint8_t data[4][4] = {
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0}
};

// Storage of seven-segment rep for certain digits
const uint8_t DIGITS[10] = {
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

// Variables to track what digit from
// what data should be displayed
volatile uint8_t type = 0;
volatile uint8_t place = 0;

bool active = true;



void cycle_digit(void) {
	if (active == false){return;}
	
	// Display the required digit
	transmitDigit(data[type][place], place);
	
	// Show the next digit along next
	place++;
	if (place == 4) {place = 0;}
}

void cycle_value(void) {
	if (active == false){return;}
		
	// Reset LEDs
	PORTB &= 0b11110000;
	PORTB |= (1 << type);
	
	type++;
	if (type == 4) {type = 0;};
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
	
	// Timer setup to CTC mode
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
	
	set_error(VOLTAGE);
	set_error(CURRENT);
	set_error(POWER);
	set_error(ENERGY);
	
	// Set display type LEDs
	DDRB |= (0b00001111);
}

void update_display_value(uint8_t *num, uint8_t decimal, uint8_t type) {
	// Stores given data in arrays of the 7 segment rep
	// 
	for (uint8_t i = 0; i < 4; i++) {
		data[type][i] = DIGITS[num[i]];
	}
	data[type][decimal] |= 0b10000000;
}

static void set_error(uint8_t type) {
	// Set a number to display an error
	data[type][0] = 0b01000000;
	data[type][1] = 0b01000000;
	data[type][2] = 0b01000000;
	data[type][3] = 0b01000000;
}

static void transmitDigit(uint8_t display, uint8_t place) {
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

void activate_7seg_display(void) {
	active = true;
	
}
void deactivate_7seg_display(void) {
	active = false;
	for (uint8_t i = 0; i < 4; i++) {
		transmitDigit(0b00000000, i);
	}
}