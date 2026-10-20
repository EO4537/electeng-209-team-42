/*
 * Measurements.c
 *
 * Created: 12/10/2026 11:31:59 am
 * Author : adity
 */ 

#include "common.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "timer1.h"
#include "zerocrossing.h"
#include "calculation.h"

float vrms;
float ipeak_ma;
float power;


int main(void){
	adc_init();
	zc_init();
	timer1_init();
	sei();

	adc_reset();
	while(1){
		if (adc_data_ready()){
			 vrms = calc_rms(v_buffer, MAX_SAMPLES, V_GAIN);               // voltage Rms in V
			 ipeak_ma = calc_peak(i_buffer, MAX_SAMPLES ,  I_GAIN) * 1000;   // Peak current in mA
			 power = calc_power(v_buffer , i_buffer , MAX_SAMPLES);
			 
			 adc_reset();
		}
	}
}

