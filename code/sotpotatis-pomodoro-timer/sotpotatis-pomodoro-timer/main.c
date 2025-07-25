/*
 *  main.c
 * Main code of the project
 */
#include "softwareConst.h"
#if RUN_TIMER_CALIBRATION == 0 && RUN_TIMER_TEST == 0
	#include "hardwareConst.h"


	#include "ledHandlers.h"

	#include "adcUtilities.h"

	#include "timerUtilities.h"

	#include <stdint.h>

	#include <avr/interrupt.h>

	#include "eepromUtilities.h"

	#include "buttonMultiplexing.h"

	// Global variables
	volatile uint32_t timestamp = 0;
	volatile uint8_t latestADCSample = 0; // The latest read sample
	volatile uint8_t latestADCSampleChecked = 1; // Whether the latest ADC sample was acknowledged by main or not.
	volatile uint8_t latestTimerTickAcknowledged = 1; // Flag that main can use to ack timer ticks

	/*
	Defines a custom ISR for ADC readings.
	*/
	ISR(TIM0_COMPA_vect) {
	  timestamp++;
	  latestTimerTickAcknowledged = 0;
	  if (timestamp % ADC_SAMPLE_RATE == 0 && latestADCSampleChecked) {
		clearADCInterrupts();
		latestADCSample = readCurrentADCValue();
		latestADCSampleChecked = 0;
	  }
	}



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

	int main(void) {
	  // Set up ADC on chosen pin, in Free Running Mode, with no Interrupts
	  // and the chosen prescaler
	  setUpADC(BUTTON_ADC_PIN, 0, 1, 0, ADC_PRESCALER_VALUE);
	  // Set up a timer in CTC mode, and generate interrupts
	  // every 1ms. Enable output compare pin on OC0A pin (PB2)
	  setPinStates(4, 1, 0);
	  setUpTimerInCTCMode(1, TICKS_PER_MS, TIMER_PRESCALER_VALUE, 1);
	  // Ensure all pins used for LED Charlieplexing are inputs.
	  resetAllCharlieplexingPins();
	  sei();
	  uint8_t currentIndex = 0;
	  uint8_t blink = 0;
	  uint8_t currentState = 0; // 0 = work, 1 = rest, 2 = repetitions
	  int8_t currentTurnedOffLED = -1;
	  uint16_t melodyTimeIndex = 0;
	  uint8_t melodyPauseIndex = 0;
	  uint8_t melodyPlaying = 1;
	  // Proof of concept code:
	  // lights up all 12 LEDs. If the user clicks the 5 button-panel,
	  // they should see the LED which corresponds to the button index that they just clicked
	  // turn off.
	  while (1) {
		// Set Charlieplexing LEDs to their current state.
		if (!latestTimerTickAcknowledged) {
			if (timestamp % 1000 == 0){
				// Blink the last LED on the panel every second
				blink = !blink;
			}
			// Turn on LEDs. Lower 12 are used for the ADC multiplexing demo. Upper 3 are used for the work / rest / repetition set
			// screen
			if (currentIndex < 12){
				if (currentTurnedOffLED != currentIndex && (currentIndex != 11 || !blink)){setCharlieplexingState(currentIndex);}
				currentIndex++;
			}
			else  {
				setCharlieplexingState(12+currentState);
				currentIndex = 0;
			}
			
			// Update the buzzer.
			// The buzzer plays a melody which is tracked using "melodyIndex". Just temporarily for testing.
			if (melodyPlaying){
				melodyTimeIndex++;
				if (melodyTimeIndex == 200){
					melodyTimeIndex = 0;
					melodyPlaying = 0;
					// Disable OCR0A to stop buzzer from sounding. 
					// Can be done in two ways - either by writing
					// to the timer register or by simply setting the pin to an input.
					// NOTE: I commented out the timer register writing method.
					// It caused a humming noise when the timer was not connected during my testing setup.
					setPinStates(4, 0, 0); 
					// setOutputComparePinA(0);
				}
			}
			else {
				if ((melodyPauseIndex  == 3 && melodyTimeIndex < 999) || (melodyPauseIndex < 3 && melodyTimeIndex < 100)){
					melodyTimeIndex++;
				}
				else {
					melodyTimeIndex = 0;
					melodyPlaying = 1;
					if (melodyPauseIndex < 3){
						melodyPauseIndex++;
					}
					else {
						melodyPauseIndex = 0;
					}
					// Reenable buzzer sound. Buzzer is connected to OCR0A pin.
					// Can be done in two ways - either by writing
					// to the timer register or by simply setting the pin to an output.
					// NOTE: I commented out the timer register writing method.
					// It caused a humming noise when the timer was not connected during my testing setup.
					setPinStates(4, 1, 0); // Set OCR0A to an output again
					// setOutputComparePinA(1);
				}
			}
			latestTimerTickAcknowledged = 1;
			// Process any new ADC samples to detect button preses
			if (!latestADCSampleChecked) {
				// No need to use a local copy of latestADCSample here as the ISR only updates it
				// when latestADCSampleChecked=1.
			 uint8_t currentADCButton = getCurrentADCButton(latestADCSample);
			updateButtonStates(buttonCounts,debouncedCounts, buttonDebounced,buttonTapped,buttonHeld, currentADCButton);
			  // Visually show on my LEDs what button that was detected as pressed
			  for (int i = 0; i < 5; i++) {
				if (buttonHeld[i] || buttonTapped[i]) {
				  buttonHeld[i] = 0;
				  buttonTapped[i] = 0;
				  currentTurnedOffLED = i;
				  // Update the current state, for illustrative purposes
					currentState++;
					if (currentState == 2){
						currentState = 0;
					}
				}
			  }
			  latestADCSampleChecked = 1;
			}
		}
		
	  }
	}
#endif