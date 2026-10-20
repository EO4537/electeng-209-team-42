/*
 * comms.c
 *
 * Created: 15/10/2026 12:40:00 PM
 *  Author: Ethan
 */

#include "comms.h"
#include "uart.h"
#include "display.h"

#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>

// A container for the number to be transmitted
// Separated into the four digits to be displayed
static uint8_t tempNum[4] = {3, 4, 5, 9};

static volatile uint8_t displayCycles = 0;
static bool active = true;

ISR (TIMER0_COMPA_vect){
	if (!active) {
		return;	
	}
	// Change the digit shown on the 7 segment display
	// every 5ms to create the illusion of a whole number
	display_cycle_digit();
	
	// After 1s, change the value on the 7 segment display
	// Transmit the current data over UART
	if (displayCycles == 200){
		displayCycles = 0;
		
		display_cycle_value();
		uart_transmit();
	}
	
	displayCycles++;
}

void comms_init(void){
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
	
	display_init();
	uart_init();
}

void comms_update_value(float num, uint8_t type){
	// Check if number is out of bounds of what can be displayed
	if (num >= 9999.5 || num < 0.001f){
		display_error(type);
		return;
	}
	
	// Scale number up or down so it can fit
	// as a 4 digit whole number (with the decimal
	// place tracked separately)
	uint8_t decimalPlace = 0;
	
	if (num >= 1){
		while (num < 999.95){
			num = num * 10;
			decimalPlace++;
		}
	} 
	else{
		num = num * 1000;
		decimalPlace = 3;
	}
	
	// Round and convert to uint16
	// This removes the decimal point
	num = num + 0.5;
	uint16_t newNum = (uint16_t) num;
	
	// Separate number into component digits
	// Store in array
	tempNum[0] = (uint8_t)(newNum % 10);
	newNum = newNum / 10;
	tempNum[1] = (uint8_t)(newNum % 10);
	newNum = newNum / 10;
	tempNum[2] = (uint8_t)(newNum % 10);
	newNum = newNum / 10;
	tempNum[3] = (uint8_t)(newNum % 10);
	newNum = newNum / 10;
	
	// Pass data into respective functions
	display_update_value(tempNum, decimalPlace, type);
	uart_update_value(tempNum, decimalPlace, type);
}

void comms_enable(void) {
	active = true;
}

void comms_disable(void) {
	active = false;
	display_blank();
}