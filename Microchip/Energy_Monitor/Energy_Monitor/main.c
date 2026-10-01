/*
 * Energy_Monitor.c
 *
 * Created: 1/10/2026 4:21:25 PM
 * Author : Ethan O'Meara
 
 NOTES:
 Timer/Counter0 is configured and being used for the display
 Timer/Counter1 should be used for any mathematics
 */ 

#include <avr/io.h>
#include "display.h"


int main(void)
{
    /* Replace with your application code */
	init_display();
    while (1) 
    {
    }
}

