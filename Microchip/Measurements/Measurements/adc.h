/*
 * adc.h
 *
 * Created: 9/09/2026 7:43:40 pm
 *  Author: adity
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <stdint.h> // Necessary for definition of uint8_t etc

#define MAX_SAMPLES 80
#define V_CHANNEL 0
#define I_CHANNEL 1

#define STATE_IDLE 0
#define STATE_READY 1
#define STATE_SAMPLING 2
#define STATE_COMPLETED 3  


extern volatile uint16_t v_buffer[MAX_SAMPLES];
extern volatile uint16_t i_buffer[MAX_SAMPLES];
extern volatile uint8_t adc_state;


void adc_init();
void adc_arm();
uint8_t adc_data_ready();


#endif /* ADC_H_ */