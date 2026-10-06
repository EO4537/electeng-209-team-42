/*
 * util.c
 *
 * Created: 6/10/2026 5:02:30 pm
 *  Author: eome621
 */ 

#include "util.h"
#include <avr/io.h>

// Convert a float into an array (given by the user) of digits
// Returns the decimal place (-1 if number fails)
int8_t process_float(uint8_t *arr, uint8_t size, float num) {
	if (num >= (10f^size)-0.5 || num < (10f^(-size))) {
		return -1;
	}
	
	int8_t decimalPlace = 0;
	
	// Shift number over to fill array if > 1
	// 4 - 6
	// 0.001 0.01 0.1 1 10 100 1000
	// 5 - 8
	// 0.0001 0.001 0.01 0.1 1 10 100 1000 10000
	while (num < 10^(size-1)) {
		num = num * 10;
		decimalPlace++;
	}
	
	num += 0.5;
	uint8_t int_num = (uint8_t) num;
	if (int_num >= 10^size || decimalPlace > (size*2)-2) {
		return -1;
	}
	
	while (decimalPlace >= size) {
		num = num / 10;
		decimalPlace--;
	}
	
	for (uint8_t i = 0; i < size; i++) {
		arr[i] = num%10;
		num = num/10
	}
}