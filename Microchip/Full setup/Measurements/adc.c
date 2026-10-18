/*
 * adc.c
 *
 * Created: 9/09/2026 7:45:25 pm
 *  Author: Aditya
 */ 

#include "common.h"
#include "adc.h"
#include "zerocrossing.h"
#include "comms.h"
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t v_buffer[MAX_SAMPLES];
volatile uint16_t i_buffer[MAX_SAMPLES];
volatile uint8_t adc_state = STATE_IDLE;

volatile uint8_t channel  = V_CHANNEL;
volatile uint8_t sample_number = 0;

void adc_init(){
	ADMUX |= (1<< REFS0) | V_CHANNEL;  // Set reference to AVCC 5V and enable V_Channel(ADC0) 
	ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADIE) | (1 << ADPS1) | (1 << ADPS0);  // Enable ADC , auto trigger , interrupt  and set prescaler to 8 for a 250kHz ADC clock
	ADCSRB |= (1 << ADTS2) | (1 << ADTS0); // Set ADC trigger source to Timer 1 compare match B
	DDRB |= (1<<PB1); // Test pin : Set PB1 as output to check ADC conversion rate
}

void adc_reset(){
	zc_flag = 0; 
	adc_state = STATE_READY;
}

uint8_t adc_data_ready(){
	if(adc_state == STATE_COMPLETED){
		return 1;
	}
	return 0;
}

ISR(ADC_vect) {
	// PORTB ^= (1<<PB1); // toggle PB1 after adc conversion is finished
	static uint8_t sample_number = 0;
	uint16_t result = ADC;  // Store ADC conversion in result 
	TIFR1 |= (1 << OCF1B);  // Clear compare flag B so next conversion works
	
	uint8_t sample_channel = channel;  // Sets whether the result came from the voltage or current channel
	
	
	// Changes between voltage and current channel so they alternate for each next conversion
	if(channel == V_CHANNEL){
		channel = I_CHANNEL;
	}
	else{
		channel = V_CHANNEL;
	}
	ADMUX = (ADMUX & 0xF0) | channel;  // Change the channel bits to use different channel for next conversion
	
	
	// Start recording samples at the first V sample after a zero crossing has occurs
	if(adc_state == STATE_READY && zc_flag == 1 && sample_channel == V_CHANNEL){
		adc_state = STATE_SAMPLING;
		sample_number= 0;
	}
	
	// Start storing adc result in the v_buffer and i_buffer
	if(adc_state == STATE_SAMPLING){
		if(sample_channel == V_CHANNEL){
				v_buffer[sample_number] = result;
		}
		else{
			i_buffer[sample_number] = result;
			sample_number++;
			if(sample_number >= MAX_SAMPLES){
				adc_state = STATE_COMPLETED;
			}
		}
	}
}
		