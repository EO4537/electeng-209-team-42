/*
 * display.h
 *
 * Created: 1/10/2026 4:22:45 PM
 *  Author: ethan
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "util.h"

void init_display(void);

void update_display_value(uint8_t *num, uint8_t decimal, uint8_t type);

void activate_7seg_display(void);

void deactivate_7seg_display(void);

#endif /* DISPLAY_H_ */