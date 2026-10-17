/*
 * Measurements.c
 *
 * Created: 12/10/2026 11:31:59 am
 * Author : Aditya
 */ 

#include "common.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"
#include "timer1.h"
#include "zerocrossing.h"
#include "calculation.h"
// added Ethan's function files
#include "comms.h"
#include "parameters.h"

float vrms;
float ipeak_ma;
float power;


int main(void){
	adc_init();
	zc_init();
	timer1_init();
	comms_init();
	sei();

	adc_reset();
	while(1){
		if (adc_data_ready()){
			 vrms = calc_rms(v_buffer, MAX_SAMPLES, V_GAIN);               // voltage Rms in V
			 ipeak_ma = calc_peak(i_buffer, MAX_SAMPLES ,  I_GAIN) * 1000;   // Peak current in mA
			 power = calc_power(v_buffer , i_buffer , MAX_SAMPLES);
			 
			 comms_update_value(vrms, VOLTAGE);
			 comms_update_value(ipeak_ma, CURRENT);
			 comms_update_value(power, POWER);
			 adc_reset();
		}
	}
}

