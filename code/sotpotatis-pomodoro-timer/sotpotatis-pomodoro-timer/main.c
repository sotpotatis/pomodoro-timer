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
#define F_CPU 8000000UL
#include <util/delay.h>
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
	// Provide a heartbeat signal on one of the pins
	// setPinStates(0x4000, 1, 0);
	// setPinStates(0x4000, 1, 1);
	// setPinStates(1<<PA0<<8, 1, 0);
	// setPinStates(1<<PA1<<8, 1, 0);
	// setPinStates(1<<PA1<<8, 1, 1);
	// setPinStates(1<<PA0<<8, 0, 1);
	while(1){
	for (int i=0;i<11;i++){
	setCharlieplexingState(i);
	_delay_ms(500);
	}}
}

