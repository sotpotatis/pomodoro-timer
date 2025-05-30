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
char value;
/*
Defines a custom ISR for ADC readings.
*/
 ISR(ADC_vect){
	// Clear ADC interrupt flag
	clearADCInterrupts();
	value = readCurrentADCValue();
	for (int i=1;i<=8;i++){
		if (value <= 32*i && (i == 1 || (i != 1 && value > 32*(i-1)))){
			setCharlieplexingState(i);
			break;
		}
	}
	
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
	// Set up ADC on PA6
	setUpADC(6, 0, 1, 1, 0b111);
	sei();
	while(1){
	}	
}

