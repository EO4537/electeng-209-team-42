/*
 * energytimeinterval.c
 *
 * Created: 19/10/2026 12:58:15 pm
 *  Author: swor614
 */ 

#include <stdint.h>
#include <math.h>
#include <avr/interrupt.h>

volatile uint8_t timer2_cycle_count;
volatile float energy_time_interval_s;
volatile float timer2cycletime_s = 0.001024; // Time that a full cycle of timer2 prescaled by 8 will take to complete in seconds
volatile float timer2steptime = 0.000004; // Time for each step when timer2 is counting

ISR(TIMER2_COMPA_vect){
	timer2_cycle_count ++;
}

void timer2_init(void){

TCCR2B |= (1 << CS21); // Sets prescaler of timer2 to 8
TCCR2A |= (1 << WGM21); // Sets counter to run in CTC mode clearing on match  
OCR2A = 255; // Sets timer to match with register A when it is just about to overflow as we are using a 8 bit timer
}


volatile float energytimeinterval (){
	uint8_t currenttimer2step = TCNT2;
	
	energy_time_interval_s = (timer2_cycle_count * timer2cycletime_s) + (currenttimer2step * timer2steptime);
	return energy_time_interval_s;
}

	
