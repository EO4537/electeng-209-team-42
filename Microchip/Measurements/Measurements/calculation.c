/*
 * calculation.c
 *
 * Created: 13/10/2026 7:57:53 am
 *  Author: adity
 */ 

#include "calculation.h"
#include <math.h>

#define VREF      5.0
#define ADC_STEPS 1024.0
#define VOFF      2.1

float calc_rms(volatile uint16_t *buffer, uint8_t n, float gain) {
	float sum=0;
	for(uint8_t i =0 ; i < n ; i++){
		float volts = (buffer[i] * VREF/ ADC_STEPS) - VOFF;  // Analogue Voltage at pin without offset
		float value = volts/gain;  // Real source voltage
		sum = sum + (value * value);  // Squaring voltage and adding them
	}
	return sqrt(sum/n);  // squareroot the whole expression
}

float calc_peak(volatile uint16_t *buffer, uint8_t n, float gain){
	uint16_t max = 0;
	for (uint8_t i = 0; i < n; i++){
		if (buffer[i] > max){
			max = buffer[i]; // Store max value
		}
	}
	float volts = (max * VREF / ADC_STEPS) - VOFF;
	return volts / gain;
}


