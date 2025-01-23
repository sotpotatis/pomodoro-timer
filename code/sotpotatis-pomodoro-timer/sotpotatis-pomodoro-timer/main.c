/*
 * (GitHub, sotpotatis) Pomodoro Timer.c
 *
 * Created: 2025-01-19 20:41:17
 * Author : sotpotatis
 */ 

#include <avr/io.h>
#include "hardwareSetup.h"
#include "hardwareConst.h"
/* 
Function to initialize the hardware.
Supposed to be called at bootup.
*/
void init(){
	enableInputPinInterrupts();
	setInitialPinStates();
}
void ledTest(){
	// LED test by using PA3 to turn on externally connected LED
	setPinStates(0x0800, 1, 0); // Set pin type to output
	setPinStates(0x0800, 1, 1); // Set pin value
}
int main(void)
{
    // Perform hardware initialization
	ledTest();	
}

