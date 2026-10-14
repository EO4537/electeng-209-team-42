
/*
 * Timer1.c
 *
 * 
 *  Author: Aditya
 */ 

#include "common.h"
#include "timer1.h"
#include <avr/io.h>



void timer1_init() {
	DDRB |= (1 << PB2); // Set PB2 as output to toggle at every sample start (Soc pin)
	TCCR1A |= (1 << COM1B0);                // Toggle OC1B on compare match B
	TCCR1B |= (1 << WGM12) | (1 << CS10);   // Timer 1 runs in CTC mode with prescaler of 1 
	OCR1A = TRIGGER_TICKS - 1;     // Set the sampling rate using OCR
	OCR1B = 100; // Compare match B triggers the ADC 
}
