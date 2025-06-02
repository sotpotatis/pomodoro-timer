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
volatile char currentCharlieplexingLED = 0;
volatile char maxCharlieplexingLed = 11;
volatile char blinkState = 0;
volatile unsigned char currentADCValue = 0;
volatile char msSinceLastSecond = 0;

/*
Defines a custom ISR for ADC readings.
*/
 ISR(TIM0_COMPA_vect){
	// Clear ADC interrupt flag
	clearADCInterrupts();
	currentADCValue = readCurrentADCValue();
	// Proof of concept that toggles all Charlieplexing LEDs on and off rapidly,
	// one by one, to appear constantly lit to the human eye
	setCharlieplexingState(currentCharlieplexingLED);
	msSinceLastSecond++;
	// Update current Charlieplexing LED
	if (msSinceLastSecond == 1000 ){
		blinkState = !blinkState;
		msSinceLastSecond = 0;
	}
	if (currentCharlieplexingLED == maxCharlieplexingLed){
		currentCharlieplexingLED = 0;
	}
	else {
		currentCharlieplexingLED++;
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
	// Set up ADC on PA6, in Free Running Mode, with no Interrupts
	// and a prescaler of 1/128
	 setUpADC(6, 0, 1, 0, 0b111);
	 // Set up a timer in CTC mode, and (unless you changed the constants from
	 // what is in the repo by default), count to 1ms, and generate interrupts
	 // every 1ms. Disable output compare pins.
	 setUpTimerInCTCMode(124,1,0b011,1);
	 sei();
	 while(1){
	 }	
}

