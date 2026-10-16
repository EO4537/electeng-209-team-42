/*
 * comms.h
 *
 * Created: 15/10/2026 12:40:21 PM
 *  Author: ethan
 */ 


#ifndef COMMS_H_
#define COMMS_H_

#include "util.h"
#include "display.h"

void comms_init(void);

void update_value(float num, uint8_t type);

void activate_display(void);

void deactivate_display(void);

#endif /* COMMS_H_ */