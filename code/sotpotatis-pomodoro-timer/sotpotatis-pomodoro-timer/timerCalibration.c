/*
 * timerCalibration.c
 * Utility code to calibrate the on-device timer. Temporary
 * replaces the main function with a simple program for timer calibration.
 * NOTE: that I am aware of this code containing some duplicated code (especially in multiplexing
 * variable setup, timer setup, ISR, and ADC). This is intended as testing code only.
 * NOTE: The timer calibration method should be credited to user "avrcandies" on AVRFreaks forums. Many thanks!
 * See this topic where it was suggested: https://www.avrfreaks.net/s/topic/a5CV40000002wfpMAA/t399784 (post 11)
 * Calibration is done like this: the user presses any button
 * to start. After the delay set by the "MINUTES" constant below, the user presses any button again.
 * Use an external stopwatch for timing <MINUTES> amount of minutes. The timer will compare the timer value
 * with the expected value and store a calibration factor in the EEPROM.
*/
	#include "softwareConst.h"
#if RUN_TIMER_CALIBRATION == 1
	#include "hardwareConst.h"
	#include "timerUtilities.h"
	#include <stdint.h>
	#include <avr/interrupt.h>
	#include "eepromUtilities.h"
	#include "adcUtilities.h"
	#include "buttonMultiplexing.h"
	#include "pinUtilities.h"
	#include "ledHandlers.h"
	// Global variables
	volatile uint32_t timestamp = 0;
	volatile uint16_t elapsedSeconds = 0;
	volatile uint8_t calibrationState = 0;
	// Define how long the calibration lasts for.
	// Calibration is done with the user pressing any button
	// to start, and then pressing any button again after "MINUTES" number
	// of minutes have elapsed in reality.
	const uint8_t MINUTES = 5;
	 // Set in main. The expected no. of seconds the timer should have counted to, assuming it counts perfectly
	 // For example, for MINUTES=5, this variable would have a value of 5 * 60
	uint16_t EXPECTED_VALUE;
	// Used for button input detection:
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
	Defines a custom ISR for ADC readings and time tracking.
	*/
	ISR(TIM0_COMPA_vect) {
		if (calibrationState == 1){
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


	int main(void) {
		EXPECTED_VALUE = 60 * MINUTES; // What timer should count to
		// Set up ADC on chosen pin, in Free Running Mode, with no Interrupts
		// and the chosen prescaler
		setUpADC(BUTTON_ADC_PIN, 0, 1, 0, ADC_PRESCALER_VALUE);
		// Ensure all pins used for LED Charlieplexing are inputs.
		resetAllCharlieplexingPins();
		// Set up a timer in CTC mode, and generate interrupts
		// every 1ms. Disable output compare pins.
		setUpTimerInCTCMode(TICKS_PER_MS, 1, TIMER_PRESCALER_VALUE, 1);
		setCharlieplexingState(2); // Turn on LED to acknowledge bootup
		sei();
		// The code below runs forever:
		// As long as calibration isn't finished (the user has pressed a button to start the calibration, then another one
		// to mark it as done), then the loop below runs. (if calibration is finished, calibrationState == 2, so the loop
		// is essentially a nop)
		while (1) {
	  		  if (!latestADCSampleChecked && calibrationState != 2){
				   uint8_t currentADCButton = getCurrentADCButton(latestADCSample);
				   updateButtonStates(buttonCounts,debouncedCounts, buttonDebounced,buttonTapped,buttonHeld, currentADCButton);
				   // Check if the user has pressed or held any button
				   for (int i =0; i < 5; i++){
					  if (buttonTapped[i] || buttonHeld[i]){
						  buttonTapped[i] = 0;
						  buttonHeld[i] = 0;
						  if (calibrationState == 0){ // If calibration hasn't started yet - start it
							  calibrationState = 1;
							  setCharlieplexingState(1); // Turn on LED to indicate start
						  }
						  else { // Stop calibration if it has already stated
							calibrationState = 2;
							setCharlieplexingState(3); // Turn on new LED to indicate end
							volatile uint16_t recordedValue = elapsedSeconds;
							// NOTE: The timer calibration method should be credited to user "avrcandies" on AVRFreaks forums. Many thanks!
							// See this topic where it was suggested: https://www.avrfreaks.net/s/topic/a5CV40000002wfpMAA/t399784 (post 11)
							// Calculate value to store in EEPROM
							recordedValue = recordedValue * 64;
							recordedValue = recordedValue / MINUTES;
							volatile uint8_t high8Bits = recordedValue >> 8;
							volatile uint8_t low8Bits = recordedValue & 0xFF;
							writeEEPROM(TIMER_CALIBRATION_EEPROM_ADDRESS_H, high8Bits);
							setCharlieplexingState(4); // Turn on third LED
							writeEEPROM(TIMER_CALIBRATION_EEPROM_ADDRESS_L, low8Bits);
						  }
					  }
				  }
				  latestADCSampleChecked = 1;
			  }
	  }
	}
#endif