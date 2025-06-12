/* adc_proof_of_concept.ino
Proof of concept for using software to sample from the ADC "resistor ladder" for button input multiplexing.
*/
// The +/- tolerance, in terms of the numeric ADC value, to use for input detection. 
// expected value for button i-VALUE_DIFF <= read ADC value <= expected value for button i+VALUE_DIFF => button i detected as pressed
// (at that instant)
int VALUE_DIFF = 25; 
// The states that the ADC can take on.
int ADC_STATES[6] = {
  1024, // No button pressed
  0, // Button 1 pressed
  208, // Button 2 pressed
  381, // Button 3 pressed
  644, // Button 4 pressed
  828 // Button 5 pressed
};
unsigned short HOLD_LIMIT_MS = 1000; // How many milliseconds of consequtively detecting the same button press until detected as held down
unsigned short CLICK_LIMIT_MS = 50; // How many milliseconds of consequtively detecting the same button press until detected as clicked

void setup() {
  Serial.begin(9600);
  Serial.println("Serial debugging began.");
}

// Global variables to track state.
char NO_BUTTON_PRESSED = 0;
char currentlyHeldDownButton = 0;
unsigned long buttonStartTimeStamp = 0;

// Get the current button that is pressed ACCORDING TO THE ADC at an INSTANTENOUS MOMENT. 
// The returns from this function is further parsed inside getCurrentPressedUserButton
// Returns 0 if no button is pressed, else a number 1-5 corresponding to the pressed button.
char getCurrentADCButton(){
  int sample = analogRead(A0);
  for (int i=0;i<6;i++){
    int mappedButtonValue = ADC_STATES[i];
    if (sample >= mappedButtonValue-VALUE_DIFF && sample <= mappedButtonValue+VALUE_DIFF){
      return i;
    }
  }
}

// Applies some simple logic to detect button presses by the user.
// Returns a number. Lower 3 bits take the decimal value 0-5 - the button number that was pressed.
// Upper 1 bit is 1 if the button was held down and 0 if it was clicked.
char getCurrentPressedUserButton(){
  char newHeldDownButton = getCurrentADCButton();
  unsigned long currentTimestamp = millis();
  char toReturn = NO_BUTTON_PRESSED;
  if (currentlyHeldDownButton != newHeldDownButton){ // If button state changed
    // "Parse" if this is a click or a press unless the current state was that no button was pressed down
    if (currentlyHeldDownButton != NO_BUTTON_PRESSED){
      Serial.print(" Button that was detected: ");
      Serial.print(currentlyHeldDownButton, DEC);
      Serial.print(", New button that was detected: ");
      Serial.print(newHeldDownButton, DEC);
      unsigned long timestampDifference = currentTimestamp - buttonStartTimeStamp;
      if (timestampDifference >= HOLD_LIMIT_MS){
        Serial.println("Button detected as hold!");
        toReturn = (0b1 | currentlyHeldDownButton);
      }
      else if (timestampDifference >= CLICK_LIMIT_MS){
        Serial.println("Button detected as short press!");
        toReturn = currentlyHeldDownButton;
      }
      else {
        Serial.println("Button detected as nothing - took too short amount of time to elapse!");
      }
    }
    currentlyHeldDownButton = newHeldDownButton;
    buttonStartTimeStamp = currentTimestamp;
  }
  return toReturn;
}

void loop() {
  // put your main code here, to run repeatedly:
  char match = getCurrentPressedUserButton();
  delay(20);
}
