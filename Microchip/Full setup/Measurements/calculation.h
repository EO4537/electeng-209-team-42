/*
 * calculation.h
 *
 * Created: 13/10/2026 7:58:08 am
 *  Author: Aditya
 */ 


#ifndef CALCULATION_H_
#define CALCULATION_H_

#include <stdint.h>
#include <math.h>
#include "common.h"
#include "timer1.h"

// Component values and Gains
#define RA 100000.00 
#define RB 4700.00 
#define RS 0.5 
#define GIO 2.8
#define GVO 1.0
#define V_GAIN ((RB / (RA + RB)) * GVO)
#define I_GAIN (RS * GIO)

// Power correction for interpolation                                
#define TRIGGER_PERIOD   (TRIGGER_TICKS / (double)F_CPU)    
#define PHASE_SHIFT      (2.0 * 3.141592654 * SIGNAL_FREQ * TRIGGER_PERIOD)   
#define POWER_CORRECTION (1.0 / cos(PHASE_SHIFT))


float calc_rms(volatile uint16_t *buffer, uint8_t n, float gain);
float calc_peak(volatile uint16_t *buffer, uint8_t n, float gain);
float calc_power(volatile uint16_t *v_buffer, volatile uint16_t *i_buffer, uint8_t n);


#endif /* CALCULATION_H_ */