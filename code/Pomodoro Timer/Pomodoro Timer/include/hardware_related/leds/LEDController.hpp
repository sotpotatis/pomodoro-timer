#pragma once

#include <stdint.h>
#include "charlieplexing.h"

class LEDController {
private:
  bool led_is_turned_on[NUMBER_OF_CHARLIEPLEXED_LEDS]; // NUMBER_OF_CHARLIEPLEXED_LEDS defined in charlieplexing.h
  int current_led_being_shown;
  bool led_number_is_valid(uint8_t led_number);
public:
  LEDController();
    
  // NOTE This doesn't (on itself) make the LED turn on physically - it just updates software state!
  // The idea is that the high-level code should use this function to control the LED state, and the change will still
  // appear almost instantly on the physical display. BUT, that is in fact handled by update_display_to_next_charlieplexing_frame
  // For that to work, you MUST have set up your code to repeatedly call update_display_to_next_charlieplexing_frame
  // (which actually does the necessary hardware updates). These requirements are described by the docstring
  // of that function.
  void turn_on_led(uint8_t led_number);
  
  // NOTE Please read the note under "turn_on_led"
  void turn_off_led(uint8_t led_number);
  
  // This function should be scheduled to run at a 1ms interval (or other short interval length), as long
  // as you want to show something on the LED screen.
  // Brief background: This project uses 5 pins to control 15 LEDs. This is possible due to a technique called
  // "Charlieplexing". To have multiple LEDs appear lit at the same time, we must light them up individually at speeds quick
  // enough to fool the eye that they're all lit at the same time!
  // Calling this function as recommended above will quickly cycle over each LED so that those who should be lit up appear
  // lit up at the same time to the eye.
  void update_display_to_next_charlieplexing_frame();
};
