/*
 * Energy_Monitor.c
 *
 * Created: 1/10/2026 4:21:25 PM
 * Author : Ethan O'Meara
 
 NOTES:
 Timer/Counter0 is configured and being used for the display
 Timer/Counter1 should be used for any mathematics
 */ 
#define F_CPU 2000000UL

#include <avr/io.h>
#include "comms.h"
#include "display.h"
#include <util/delay.h>

int main(void)
{
    /* Replace with your application code */
	comms_init();
	update_value(12.34, VOLTAGE);
	update_value(123.4, CURRENT);
	update_value(0.0321, POWER);
	update_value(8888, ENERGY);
	_delay_ms(2000);
	deactivate_display();
	_delay_ms(2000);
	activate_display();
    while (1)
    {
		
    }
}

