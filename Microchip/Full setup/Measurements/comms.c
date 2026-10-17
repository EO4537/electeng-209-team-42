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

uint8_t tempNum[4] = {3, 4, 5, 9};
	
volatile uint8_t displayCycles = 0;

ISR (TIMER0_COMPA_vect){
	cycle_digit();
	
	// Move to different display if 1s passed
	displayCycles++;
	if (displayCycles == 200){
		displayCycles = 0;
		
		cycle_value();
		transmit_data();
	}
}

void comms_init(void){
	init_display();
	init_uart();
}

void update_value(float num, uint8_t type){
	// Check if number is out of bounds of what can be displayed
	/*if (num >= 9999.5 || num < 0.001f){
		set_error(type);
		return;
	}*/
	
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
	update_display_value(tempNum, decimalPlace, type);
	update_uart_value(tempNum, decimalPlace, type);
}

void activate_display(void) {
	activate_7seg_display();
}

void deactivate_display(void) {
	deactivate_7seg_display();
}