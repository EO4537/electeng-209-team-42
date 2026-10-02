/*
 * display.h
 *
 * Created: 1/10/2026 4:22:45 PM
 *  Author: ethan
 */ 


#ifndef DISPLAY_H_
#define DISPLAY_H_

#define VOLTAGE 0
#define CURRENT 1
#define POWER 2

void init_display(void);

void update_value(float num, uint8_t type);



#endif /* DISPLAY_H_ */