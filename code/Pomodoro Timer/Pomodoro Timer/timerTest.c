/*
 * timerTest.c
 * Utility code to test the timer based on a calibrated value set by running
 * timerCalibration.c.
 * NOTE: that I am aware of this code containing some duplicated code (especially in multiplexing
 * variable setup, timer setup, ISR, and ADC). This is intended as testing code only.
 * NOTE: The timer calibration method should be credited to user "avrcandies" on AVRFreaks forums. Many thanks!
 * See this topic where it was suggested: https://www.avrfreaks.net/s/topic/a5CV40000002wfpMAA/t399784 (post 11)
 * Test is done like this: Press any button connected to the microcontroller. This starts a countdown of
 * MINUTES_TO_COUNT_TO minutes. A single LED will turn on and ack that the countdown has started. Be quick and start a stopwatch!
 * When the timer has reached MINUTES_TO_COUNT_TO minutes, all of its LEDs will turn on. Stop the stopwatch!
 * Now you can see if the timer is off time-wise or not.
 */

#include "softwareConst.h"
#if RUN_TIMER_TEST == 1
	#include <stdint.h>
	#include "timerUtilities.h"
	#include "hardwareConst.h"
	#include "ledHandlers.h"
	#include <math.h>
	#include <avr/interrupt.h>
	#include "eepromUtilities.h"
	#include "adcUtilities.h"
	#include "buttonMultiplexing.h"
	// Time-keeping variables
	volatile uint8_t timerRunning = 0;
	volatile uint32_t timestamp = 0;
	volatile uint16_t elapsedSeconds = 0;
	uint8_t MINUTES_TO_COUNT_TO = 63; // No. of minutes to count to for this test
    // ADC multiplexing variables
	volatile uint8_t latestADCSampleChecked = 1;
	volatile uint8_t latestADCSample = 0;
	// Store a numeric counter for each button that, for each reading: counts up (+1) if the ADC detects a voltage reading mapped to that button.
	// It counts down (-1) for all buttons that the ADC are not mapped to.
	uint8_t buttonCounts[5] = {
		0,
		0,
		0,
		0,
		0
	};
	// Same as buttonCounts but for hold detection
	uint8_t debouncedCounts[5] = {
		0,
		0,
		0,
		0,
		0
	};
	// Store whether buttons were debounced or not. See below for usage.
	uint8_t buttonDebounced[5] = {
		0,
		0,
		0,
		0,
		0
	};
	// Store whether buttons were detected as held or not.
	// These flags are cleared by main.
	uint8_t buttonHeld[5] = {
		0,
		0,
		0,
		0,
		0
	};
	// Store whether buttons were detected as tapped or not.
	// These flags are cleared by main.
	uint8_t buttonTapped[5] = {
		0,
		0,
		0,
		0,
		0
	};
	/*
	Defines a custom ISR which counts the time elapsed.
	*/
	ISR(TIM0_COMPA_vect) {
		if (timerRunning == 1){
			timestamp++;
			if (timestamp % 1000 == 0){
				elapsedSeconds++;
			}
		}
		 if (timestamp % ADC_SAMPLE_RATE == 0 && latestADCSampleChecked) {
			 clearADCInterrupts();
			 latestADCSample = readCurrentADCValue();
			 latestADCSampleChecked = 0;
		 }
	}
		int main(){
			// Set up ADC and timer, with timer interrupts every 1ms
			setUpADC(BUTTON_ADC_PIN, 0, 1, 0, ADC_PRESCALER_VALUE);
			setUpTimerInCTCMode(0, TICKS_PER_MS, TIMER_PRESCALER_VALUE, 1);
			sei();
			// Calculate how many seconds we should count to, based on the timer calibration factor.
			// NOTE: The timer calibration method should be credited to user "avrcandies" on AVRFreaks forums. Many thanks!
			// See this topic where it was suggested: https://www.avrfreaks.net/s/topic/a5CV40000002wfpMAA/t399784 (post 11)
			// Calibration value is stored in EEPROM
			uint16_t calibrationValueH = readEEPROM(TIMER_CALIBRATION_EEPROM_ADDRESS_H);
			uint8_t calibrationValueL = readEEPROM(TIMER_CALIBRATION_EEPROM_ADDRESS_L);
			uint32_t calibrationValue = (calibrationValueH<<8) | (calibrationValueL);
			uint16_t secondsToCountTo = (MINUTES_TO_COUNT_TO * calibrationValue) >> 6;
			while (1){
				// Allow the user to start the timer by clicking on any button
				if (!latestADCSampleChecked && !timerRunning){
					uint8_t currentADCButton = getCurrentADCButton(latestADCSample);
					updateButtonStates(buttonCounts,debouncedCounts, buttonDebounced,buttonTapped,buttonHeld, currentADCButton);
					for (int i =0; i < 5; i++){
						if (buttonTapped[i] || buttonHeld[i]){
							buttonTapped[i] = 0;
							buttonHeld[i] = 0;
							timerRunning = 1;
							setCharlieplexingState(0); // Turn on LED to acknowledge countdown start
						}
					}
					latestADCSampleChecked = 1;
				}
				// Check timeout
				if (elapsedSeconds >= secondsToCountTo){
					// On timeout, turn on all LEDs.
					for (int i=0;i<15;i++){
						setCharlieplexingState(i);
					}
				}
			}
		}
#endif