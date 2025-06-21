/* adc_proof_of_concept.ino
Proof of concept for using software to sample from the ADC "resistor ladder" for button input multiplexing.
*/

// The states that the ADC can take on.
int ADC_STATES[6] = {
  926, // No button pressed
  736, // Button 1 pressed
  513, // Button 2 pressed
  295, // Button 3 pressed
  104, // Button 4 pressed
  0 // Button 5 pressed
};

unsigned char btn_counts[5] = {
  0,
  0,
  0,
  0,
  0
};
unsigned char btn_debounced[5] = {
  0,
  0,
  0,
  0,
  0
};
unsigned long btn_timestamps[5] = { // Store timestamps of when buttons were clicked.
  0,
  0,
  0,
  0,
  0
};
// Constants
unsigned char ADC_SAMPLE_RATE = 5; // The millisecond delay between each ADC sample
unsigned short HOLD_LIMIT_MS = 1000; // How many milliseconds of consequtively detecting the same button press until detected as held down
unsigned short CLICK_LIMIT_MS = 60; // How many milliseconds of consequtively detecting the same button press until detected as clicked
// The value that max_btn_counts for a certain button should reach in order to detect it as held down or clicked.
unsigned char SAMPLE_LIMIT = CLICK_LIMIT_MS / (2 * ADC_SAMPLE_RATE); // How many consequtive ADC samples that needs to be read before detecting a button as pressed.
// Global variables to track state.
char NO_BUTTON_PRESSED = 0;
void setup() {
  Serial.begin(9600);
  Serial.println("Serial debugging began.");
}



// Get the current button that is pressed ACCORDING TO THE ADC at an INSTANTENOUS MOMENT. 
// The returns from this function is further parsed inside getCurrentPressedUserButton
// Returns 0 if no button is pressed, else a number 1-5 corresponding to the pressed button.
char getCurrentADCButton(){
  int sample = analogRead(A0);
  for (int i=0;i<6;i++){
    int mappedButtonValue = ADC_STATES[i];
    if (sample >= mappedButtonValue){
      return i;
    }
  }
}

// Applies some simple logic to detect button presses by the user.
// Returns a number. Lower 3 bits take the decimal value 0-5 - the button number that was pressed.
// Upper 1 bit is 1 if the button was held down and 0 if it was clicked.
char getCurrentPressedUserButton(){
  char currentADCButton = getCurrentADCButton();
  unsigned long ts = millis();
  for (int i=1; i<=5;i++){
      // Logic: increase the "btn_count" for each button by 1 every time
      // it is detected by the ADC as pressed. Decrease the btn_count for all other
      // buttons.
      if (i == currentADCButton &&  btn_counts[i-1] <= SAMPLE_LIMIT){
          btn_counts[i-1]++;
          // Store the time when a button was detected as depressed
          if (btn_counts[i-1] == SAMPLE_LIMIT && !btn_debounced[i-1]){
            btn_timestamps[i-1] = ts;
            btn_debounced[i-1] = 1;
          }
      }
      // Decrement state of other buttons
      else if (btn_counts[i-1] != 0) {
        btn_counts[i-1]--;
        // If button state just ticked over to 0, we've detected a press
        if (btn_counts[i-1] == 0){
          Serial.print("State of button ");
          Serial.print(i);
          Serial.print(" changed: Detected state: ");
          unsigned long deltaT = ts - btn_timestamps[i-1];
          btn_debounced[i-1] = 0;
          // Detect hold.
          if (deltaT  >= HOLD_LIMIT_MS){
            Serial.println("Held down!");
            return (8 | i);
          }
          // Detect click.
          else if (deltaT >= CLICK_LIMIT_MS){
            Serial.println("Clicked!");
            return i;
          }
          // If the button was not pressed down enough.
          else {
            Serial.println("No press!");
          }
          btn_timestamps[i-1] = 0;
        }
      }

  }
  return NO_BUTTON_PRESSED;
}

void loop() {
  char match = getCurrentPressedUserButton();
  delay(ADC_SAMPLE_RATE);
}
