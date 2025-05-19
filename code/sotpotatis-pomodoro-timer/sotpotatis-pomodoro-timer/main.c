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
#include "adcUtilities.h"
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

/*
Defines a custom ISR for ADC readings.
*/
ISR(ADC_vect){
	// Clear ADC interrupt flag
	clearADCInterrupts();
}
/* 
Function to initialize the hardware.
Supposed to be called at bootup.
*/
void init(){
	// Set up ADC for the button multiplexing.
}
void ledTest(){
	
}
int main(void)
{
	setUpADC(1, 0, 1, 1, 0b111);
	sei();
	while(1){
		waitForInterrupts();
	}
	
	
}

