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
#include "display.h"
#include <util/delay.h>

int main(void)
{
    /* Replace with your application code */
	init_display();
	_delay_ms(2000);
	update_value(9999.4, VOLTAGE); // Largest number possible
	update_value(0.001, CURRENT); // Smallest number possible
	update_value(12.34, POWER);
    while (1) 
    {
		
    }
}

