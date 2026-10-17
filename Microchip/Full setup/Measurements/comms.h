/*
 * comms.h
 *
 * Created: 15/10/2026 12:40:21 PM
 *  Author: Ethan
 */ 


#ifndef COMMS_H_
#define COMMS_H_

#include "util.h"
#include "display.h"

#define VOLTAGE 0
#define CURRENT 1
#define POWER 2
#define ENERGY 3

void comms_init(void);

void comms_update_value(float num, uint8_t type);

void comms_enable(void);

void comms_disable(void);

#endif /* COMMS_H_ */