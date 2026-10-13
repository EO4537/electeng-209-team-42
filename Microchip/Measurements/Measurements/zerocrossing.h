/*
 * zerocrossing.h
 *
 * Created: 12/10/2026 12:44:57 pm
 *  Author: adity
 */ 


#ifndef ZEROCROSSING_H_
#define ZEROCROSSING_H_

#include <stdint.h>

#define WHOLECYCLES 10 // number of whole samples in one period

extern volatile uint8_t zc_flag;  // flag for detecting start of zero crossing

void zc_init();




#endif /* ZEROCROSSING_H_ */