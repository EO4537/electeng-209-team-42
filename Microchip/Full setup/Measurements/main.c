/*
 * Measurements.c
 *
 * Created: 12/10/2026 11:31:59 am
 * Author : Aditya
 */ 

#include "common.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "adc.h"
#include "timer1.h"
#include "zerocrossing.h"
#include "calculation.h"
// added Ethan's function files
#include "comms.h"
#include "parameters.h"
#include "energytimeinterval.h"

float vrms;
float ipeak_ma;
float power;
float irms;
float calculatedenergy; 

int main(void){
	adc_init();
	zc_init();
	timer1_init();
	timer2_init();
	comms_init();
	sei();

	comms_disable();
	adc_reset();
	while(1){
		if (adc_data_ready()){
			comms_enable();
			 vrms = calc_rms(v_buffer, MAX_SAMPLES, V_GAIN);               // voltage Rms in V
			 //irms = calc_rms(i_buffer, MAX_SAMPLES, I_GAIN);
			 ipeak_ma = calc_peak(i_buffer, MAX_SAMPLES ,  I_GAIN) * 1000;   // Peak current in mA
			 power = calc_power(v_buffer , i_buffer , MAX_SAMPLES) + (RS*calc_rms(i_buffer, MAX_SAMPLES, I_GAIN));
			 calculatedenergy = calculatedenergy + (power * energytimeinterval());
			 
			 comms_update_value(vrms+0.2, VOLTAGE);
			 comms_update_value(ipeak_ma+40, CURRENT);
			 comms_update_value(power, POWER);
			 comms_update_value(calculatedenergy, ENERGY);
			 
			 comms_disable();
			 //_delay_ms(200);
			 adc_reset();
		}
	}
}

