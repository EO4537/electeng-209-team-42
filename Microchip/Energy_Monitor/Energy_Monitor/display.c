/*
 * display.c
 *
 * Created: 1/10/2026 4:22:26 PM
 *  Author: ethan
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "display.h"

// Defining pins
#define SH_CP (1 << PORTC3)
#define SH_DS (1 << PORTC4)
#define SH_ST (1 << PORTC5)

// Internal functions not used by main.c
static void transmitDigit(uint8_t num, uint8_t place);
static void update_int_value(uint16_t num, uint8_t type, uint8_t decimalPlace);
static void set_error(uint8_t type);

// Current data to display, stored as each digits seven-segment representation
uint8_t data[3][4] = {
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
volatile uint8_t cycles = 0;
volatile uint8_t type = 0;
volatile uint8_t place = 0;

ISR (TIMER0_COMPA_vect) {
	// Display the required digit
	transmitDigit(data[type][place], place);
	
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
	
	// Set display type LEDs
	DDRB |= (0b00001111);
}

void update_value(float num, uint8_t type) {
	// Check if number is out of bounds of what can be displayed
	if (num >= 9999.5 || num < 0.001f) {
		set_error(type);
		return;
	}
	
	// Shift number over until it fills display
	// Track where decimal point should go
	uint8_t deicimalPlace = 0;
	
	if (num >= 1) {
		while (num < 999.95) {
			num = num * 10;
			deicimalPlace++;
		}
	} else {
		num = num * 1000;
		deicimalPlace = 3;
	}
	
	num = num + 0.5;
	update_int_value((uint16_t) num, type, deicimalPlace);
}

static void update_int_value(uint16_t num, uint8_t type, uint8_t decimalPlace) {
	// Split 4 digit number into the separate digits
	data[type][0] = DIGITS[num%10];
	num = num/10;
	data[type][1] = DIGITS[num%10];
	num = num/10;
	data[type][2] = DIGITS[num%10];
	num = num/10;
	data[type][3] = DIGITS[num%10];
	
	data[type][decimalPlace] |= 0b10000000;
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