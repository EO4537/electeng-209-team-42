/*
 * calculation.c
 *
 * Created: 13/10/2026 7:57:53 am
 *  Author: Aditya
 */ 

#include "calculation.h"
#include <math.h>

float calc_rms(volatile uint16_t *buffer, uint8_t n, float gain){
	float sum=0;
	for(uint8_t i =0 ; i < n ; i++){
		float volts = (buffer[i] * VREF/ ADC_STEPS) - VOFF;  // Analogue Voltage at pin without offset
		float value = volts/gain;  // Real source voltage
		sum = sum + (value * value);  // Squaring voltage and adding them
	}
	return sqrt(sum/n);  // Squareroot the whole expression
}

float calc_peak(volatile uint16_t *buffer, uint8_t n, float gain){
	uint16_t max = 0;
	for (uint8_t i = 0; i < n; i++){
		if (buffer[i] > max){
			max = buffer[i]; // Store max value
		}
	}
	float volts = (max * VREF / ADC_STEPS) - VOFF;
	return (volts / gain);
}

static float counts_to_value(uint16_t counts , float gain){
	float volts = (counts * VREF / ADC_STEPS) - VOFF;
	return volts/ gain;
}

float calc_power(volatile uint16_t *v_buffer, volatile uint16_t *i_buffer, uint8_t n){
	float sum=0;
	
	//sum = sum + ((counts_to_value(v_buffer[9], V_GAIN) + counts_to_value(v_buffer[10], V_GAIN))/2 * counts_to_value(i_buffer[0], I_GAIN));
	
	for (uint8_t i =1; i < n-1; i++){  // Skip fist and last sample so every term can be calculated
		float v_now  = counts_to_value(v_buffer[i], V_GAIN);
		float v_next = counts_to_value(v_buffer[i + 1], V_GAIN);
		float i_previous = counts_to_value(i_buffer[i - 1], I_GAIN);
		float i_now  = counts_to_value(i_buffer[i], I_GAIN);
		
		float v_interpolated = (v_now + v_next) / 2;  // V at the instant of I[i]
		float i_interpolated = (i_previous + i_now) / 2; // I at the instant of V[i]
		
		sum = sum + (v_now * i_interpolated) + (v_interpolated * i_now);
	}
	
	float power = sum / (float)(2*(n-1));
	
	return power; 
	//return power * POWER_CORRECTION;
	
}
