/*
 * display.h
 *
 * Created: 1/10/2026 4:22:45 PM
 *  Author: Ethan
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "util.h"

void display_init(void);

void display_update_value(uint8_t *num, uint8_t decimal, uint8_t type);

void display_blank(void);

void display_error(uint8_t type);

void display_cycle_digit(void);

void display_cycle_value(void);

#endif /* DISPLAY_H_ */