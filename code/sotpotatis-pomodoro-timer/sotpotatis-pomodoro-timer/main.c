/*
 * (GitHub, sotpotatis) Pomodoro Timer.c
 *
 * Created: 2025-01-19 20:41:17
 * Author : sotpotatis
 */ 

#include <avr/io.h>
#include "hardwareSetup.h"
#include "hardwareConst.h"
#include "ledHandlers.h"
/* 
Function to initialize the hardware.
Supposed to be called at bootup.
*/
void init(){
}
void ledTest(){
	
}
int main(void)
{
    // Perform hardware initialization

	init();
	setPinStates(0x4000, 1, 0);
	setPinStates(0x4000, 1, 1);
	setCharlieplexingState(1);
}

