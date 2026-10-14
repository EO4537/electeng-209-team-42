/*
 * zerocrossing.c
 *
 * Created: 12/10/2026 12:46:02 pm
 *  Author: aditya 
 */ 

#include "adc.h"
#include "zerocrossing.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t zc_flag = 0;


void zc_init(){
	DDRD &= ~(1 << PD2);   //  Set PD2 (INT0) as input
	EICRA |= (1 << ISC01) | (1 << ISC00);   // Rising edge on INT0 generates an interrupt
	EIMSK |= (1 << INT0);     // Enable INT0
}

ISR(INT0_vect){
	if (adc_state == STATE_READY){
		zc_flag =1;
	}
}