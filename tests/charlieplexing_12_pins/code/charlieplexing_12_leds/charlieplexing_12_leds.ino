/* charlieplexing_12_leds.ino
Test Arduino sketch (to run on an Arduino UNO, not Attinys)
for testing 4 pin Charlieplexing, in my case controlling 12 LEDs.
I wanted to use this in my project.
*/
// All GPIO pins in use.
const char TOTAL_PINS = 4;
 const char gpioPins[TOTAL_PINS] = {
   0,
   1,
   2,
   3
 };
// Contains the pins to set to output HIGH and output LOW to lit each of the 12 LEDs
// Led i is represented by index i in this map, and is in the format {pin to set to low, pin to set to high}
// Other pins are input pins.
// My schematic pins are called JP1, JP2, JP3, JP4, correspodning to GPIO pin number 1, 2, 3 and 4.
const char allPinStates[12][2] = {
  {gpioPins[1], gpioPins[0]}, // LED1  -> JP2 LOW, JP1 HIGH
  {gpioPins[0], gpioPins[1]}, // LED2  -> JP1 LOW, JP2 HIGH
  {gpioPins[2], gpioPins[0]}, // LED3  -> JP3 LOW, JP1 HIGH
  {gpioPins[0], gpioPins[2]}, // LED4  -> JP1 LOW, JP3 HIGH
  {gpioPins[3], gpioPins[0]}, // LED5  -> JP4 LOW, JP1 HIGH
  {gpioPins[0], gpioPins[3]}, // LED6  -> JP1 LOW, JP4 HIGH
  {gpioPins[2], gpioPins[1]}, // LED7  -> JP3 LOW, JP2 HIGH
  {gpioPins[1], gpioPins[2]}, // LED8  -> JP2 LOW, JP3 HIGH
  {gpioPins[3], gpioPins[1]}, // LED9  -> JP4 LOW, JP2 HIGH
  {gpioPins[1], gpioPins[3]}, // LED10 -> JP2 LOW, JP4 HIGH
  {gpioPins[3], gpioPins[2]}, // LED11 -> JP4 LOW, JP3 HIGH
  {gpioPins[2], gpioPins[3]}  // LED12 -> JP3 LOW, JP4 HIGH
};
unsigned long startMillis = 0;
 // Later intended to be used as a progress bar for the timer,
 // to indicate time left, by only having turned on for example LED 1,2,3, and 4.
 char numberOfLEDsOn = 12;
 // Used to blink the rightmost LED on the progressbar every second,
 // keeping all other LEDs before it on, to indicate that the timer
 // is counting down.
 unsigned long microsecondsElapsed = 0;
 char blinkState = 0;
 // Set all pins to inputs. Used for clearing the LEDs.
 void setAllPinsToInputs(){
   for (int i=0;i<TOTAL_PINS;i++){
      pinMode(gpioPins[i], INPUT);
   }
 }
// Given a pointer to a 2-element char array in the format {pin to set to low, pin to set to high},
// sets one of the pins to low and the other one to high.
 void setPinState(const char* pinState){
   for (int i=0;i<2;i++){
     const char pin = *pinState;
     pinMode(pin, OUTPUT);
     digitalWrite(pin, i == 1);
     pinState++;
   }
 }
 void setup() {
   // Set up pins
   setAllPinsToInputs();
 }
 void loop() {
   // Set all pins to input
   // Turn on all LEDs up until the LED to blink (see comments around "timeElapsed") above
   char upperLimit = numberOfLEDsOn-1;
   if (blinkState){
     upperLimit++;
   }
   for (int i = 0; i < upperLimit; i++) {
     // For each pin state, set the LEDs accordingly
     setPinState(allPinStates[i]);
     delayMicroseconds(1000);
     microsecondsElapsed += 1000;
     setAllPinsToInputs();
  }
  // Update blinkState every second
  if (microsecondsElapsed >= 1e6){
    microsecondsElapsed = 0;
    blinkState = !blinkState;
  }
 }