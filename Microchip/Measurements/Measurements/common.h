/*
 * common.h
 *
 * Created: 9/09/2026 7:48:37 pm
 *  Author: adity
 */ 


#ifndef COMMON_H_
#define COMMON_H_
#define F_CPU 2000000UL
#include "util/delay.h" 
#define BAUD 9600
#define UBRR ((F_CPU/(16UL*BAUD)) - 1)




#endif /* COMMON_H_ */