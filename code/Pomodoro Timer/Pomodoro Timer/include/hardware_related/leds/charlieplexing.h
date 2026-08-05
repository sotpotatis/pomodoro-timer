#pragma once
// NOTE that the underlying implementation (see src/) needs to change if this number increases.
const uint8_t NUMBER_OF_CHARLIEPLEXED_LEDS = 15;
extern "C" {
	// NOTE turn_on_charlieplexed_led will not only turn on the LED with the led_number, 
	// it will also turn off any other LED that might be turned on (only 1 LED is lit at a time).
	// led_number is a number between 0-NUMBER_OF_CHARLIEPLEXED_LEDS (constant defined above in
	// same header file)
	void turn_on_charlieplexed_led(uint8_t led_number);
	
	void turn_off_all_charlieplexed_leds();
}
