/*
 *  main.c
* Main code of the project
 */

#include <avr/io.h>

#include "hardwareSetup.h"

#include "hardwareConst.h"

#include "ledHandlers.h"

#define F_CPU 8000000UL

#include <util/delay.h>

#include "adcUtilities.h"


#include <stdint.h>
#include <avr/interrupt.h>


// Global variables
volatile uint32_t timestamp = 0;
volatile uint8_t latestADCSample = 0; // The latest read sample
volatile uint8_t latestADCSampleChecked = 1; // Whether the latest ADC sample was acknowledged by main or not.


// Constants
uint8_t ADC_SAMPLE_RATE = 5; // The millisecond delay between each ADC sample
// How many consecutive ADC samples that needs to be read before detecting a button as clicked.
uint8_t CLICK_SAMPLE_LIMIT = 10;
// How many consecutive reads that a button must be declared debounced = true for it to be detected as 
// long pressed
uint8_t HOLD_SAMPLE_LIMIT = 100;


/*
Defines a custom ISR for ADC readings.
*/
ISR(TIM0_COMPA_vect) {
  timestamp++;
  if (timestamp % ADC_SAMPLE_RATE == 0 && latestADCSampleChecked) {
    clearADCInterrupts();
    latestADCSample = readCurrentADCValue();
    latestADCSampleChecked = 0;
  }
}


// Defines, for each button/circuit state, an ADC reading that if exceeded (or exactly achieved) (>=) results in that
// state being detected (instantaneously).
uint8_t ADC_STATES[6] = {
  232, // No button pressed
  185, // Button 1 pressed
  129, // Button 2 pressed
  75, // Button 3 pressed
  27, // Button 4 pressed
  0 // Button 5 pressed
};

// Store a numeric counter for each button that, for each reading: counts up (+1) if the ADC detects a voltage reading mapped to that button.
// It counts down (-1) for all buttons that the ADC are not mapped to.
uint8_t btn_counts[5] = { 
  0,
  0,
  0,
  0,
  0
};
// Same as btn_counts but for hold detection
uint8_t btn_hold_count[5] = { 
  0,
  0,
  0,
  0,
  0
};
// Store whether buttons were debounced or not. See below for usage.
uint8_t btn_debounced[5] = { 
  0,
  0,
  0,
  0,
  0
};
// Store whether buttons were detected as held or not.
// These flags are cleared by main.
uint8_t btn_held[5] = { 
  0,
  0,
  0,
  0,
  0
};
// Store whether buttons were detected as tapped or not.
// These flags are cleared by main.
uint8_t btn_tapped[5] = {
	0,
	0,
	0,
	0,
	0
};
// Get the current button that is pressed ACCORDING TO THE ADC at an INSTANTENOUS MOMENT.
// The returns from this function is further parsed inside getCurrentPressedUserButton
// Returns 0 if no button is pressed, else a number 1-5 corresponding to the pressed button.
uint8_t getCurrentADCButton() {
  for (int i = 0; i < 6; i++) {
    uint8_t mappedButtonValue = ADC_STATES[i];
    if (latestADCSample >= mappedButtonValue) {
      return i;
    }
  }
}
// Updates the detected state (pressed, tapped, or nothing) for currentADCButton.
void updateButtonStates(uint8_t currentADCButton) {
  for (int i = 0; i < 5; i++) {
    // Logic: increase the "btn_count" for each button by 1 every time
    // it is detected by the ADC as pressed. Decrease the btn_count for all other
    // buttons.
    if (i+1 == currentADCButton) {
		if (!btn_debounced[i]){
			if (btn_counts[i] < CLICK_SAMPLE_LIMIT){
				btn_counts[i]++;
			}
			if (btn_counts[i] == CLICK_SAMPLE_LIMIT) {
				btn_debounced[i] = 1;
			}
		}
    }
    // Decrement btn_counts for undetected buttons
    else {
		if (btn_counts[i] > 0){
			btn_counts[i]--;
		}
		// Reset state if ticked over to 0
		if (btn_counts[i] == 0){
			// Detect tapped (not held)
			if (btn_debounced[i] && !btn_held[i]){
				btn_tapped[i] = 1; 
			}
			btn_hold_count[i] = 0;
			btn_debounced[i] = 0;
		}
    }
	// Perform hold detection for any debounced button
	if (btn_debounced[i]) {
		if (btn_hold_count[i] < HOLD_SAMPLE_LIMIT){
			btn_hold_count[i]++;
		}
		if (btn_hold_count[i] == HOLD_SAMPLE_LIMIT) {
			btn_held[i] = 1;
		}
	}
  }
}
int main(void) {
  // Set up ADC on PA6, in Free Running Mode, with no Interrupts
  // and a prescaler of 1/128
  setUpADC(6, 0, 1, 0, 0b111);
  // Set up a timer in CTC mode, and generate interrupts
  // every 1ms. Disable output compare pins.
  setUpTimerInCTCMode(124, 1, 0b011, 1);
  sei();
  uint8_t currentADCButton = 0;
  while (1) {
    if (!latestADCSampleChecked) {
      currentADCButton = getCurrentADCButton();
      updateButtonStates(currentADCButton);
      // Visually show on my LEDs what button that was detected as pressed
      // & 7 is since I only show the button number for now by lighting up its corresponding LED,
      // and bit 4 indicates hold / not hold. See getCurrentPressedUserButton docstring.
      for (int i = 0; i < 5; i++){
        if (btn_held[i]){
			setCharlieplexingState(i);
			btn_held[i] = 0;
        }
		if (btn_tapped[i]){
			setCharlieplexingState(i);
			btn_tapped[i] = 0;
      }
    }
	latestADCSampleChecked = 1;
  }
}
}