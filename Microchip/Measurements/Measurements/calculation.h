/*
 * calculation.h
 *
 * Created: 13/10/2026 7:58:08 am
 *  Author: adity
 */ 


#ifndef CALCULATION_H_
#define CALCULATION_H_

#include <stdint.h>

#define RA 100000.00 
#define RB 4700.00 
#define RS 0.5 
#define GIO 2.8
#define GVO 1.0

#define V_GAIN ((RB / (RA + RB)) * GVO)
#define I_GAIN (RS * GIO)


float calc_rms(volatile uint16_t *buffer, uint8_t n, float gain);
float calc_peak(volatile uint16_t *buffer, uint8_t n, float gain);


#endif /* CALCULATION_H_ */