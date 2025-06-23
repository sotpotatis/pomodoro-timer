/* adcMultiplexing.c
Source code of getCurrentADCButton and updateButtonStates functions before they were translated to Assembly. 
Their Assembly translations have made it into the final source code, see adcMultiplexing.S.
*/
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
// Get the current button that is pressed ACCORDING TO THE ADC at an INSTANTENOUS MOMENT.
// The returns from this function is further parsed inside getCurrentPressedUserButton
// Returns 0 if no button is pressed, else a number 1-5 corresponding to the pressed button.
uint8_t getCurrentADCButton() {
  volatile uint8_t sampleCopy = latestADCSample;
  for (int i = 0; i < 6; i++) {
    uint8_t mappedButtonValue = ADC_STATES[i];
    if (sampleCopy >= mappedButtonValue) {
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
    if (i + 1 == currentADCButton) {
      if (buttonCounts[i] < DEBOUNCE_SAMPLE_LIMIT) {
        buttonCounts[i]++;
      } else {
        buttonDebounced[i] = 1;
      }
    }
    // Decrement buttonCounts for undetected buttons
    else {
      if (buttonCounts[i] > 0) {
        buttonCounts[i]--;
      }
      // Reset state if ticked over to 0
      else {
        buttonDebounced[i] = 0;
      }
    }
    // Perform hold detection for any debounced button
    if (buttonDebounced[i]) {
      if (debouncedCounts[i] < HOLD_SAMPLE_LIMIT) {
        debouncedCounts[i]++;
        if (debouncedCounts[i] == HOLD_SAMPLE_LIMIT){
          buttonHeld[i] = 1;
        }
      }
    } else {
      // Detect tapped (not held)
      if (debouncedCounts[i] >= 1 && debouncedCounts[i] < TAP_SAMPLE_LIMIT) {
        buttonTapped[i] = 1;
      }
      debouncedCounts[i] = 0;
    }
  }
}