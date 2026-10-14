/*
 * common.h
 *
 * Created: 9/09/2026 7:48:37 pm
 *  Author: adity
 */ 


#ifndef COMMON_H_
#define COMMON_H_
// MCU
#define F_CPU 2000000UL
#include "util/delay.h" 

// UART
#define BAUD 9600
#define UBRR ((F_CPU/(16UL*BAUD)) - 1)

#define SIGNAL_FREQ 500          

// ADC conversion constants
#define VREF      5.0              
#define ADC_STEPS 1024.0           
#define VOFF      2.1
#define SAMPLING_RATE 9500UL // ADC conversion rate




#endif /* COMMON_H_ */