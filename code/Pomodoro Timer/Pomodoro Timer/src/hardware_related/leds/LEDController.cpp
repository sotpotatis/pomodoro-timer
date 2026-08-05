#include "../../../include/hardware_related/leds/LEDController.hpp"
#include "../../../include/hardware_related/leds/charlieplexing.h"

LEDController::LEDController() : current_led_being_shown(0) {
	for (int i=0; i< NUMBER_OF_CHARLIEPLEXED_LEDS; i++){
		led_is_turned_on[i] = false;
	}
}

bool LEDController::led_number_is_valid(uint8_t led_number){
  // The type of led_number (an *unsigned* int) ensures that the constraint
  // led_number >= 0 holds, no need for an explicit check.
  return led_number < NUMBER_OF_CHARLIEPLEXED_LEDS;
}

void LEDController::turn_on_led(uint8_t led_number){
if (this->led_number_is_valid(led_number)){
	led_is_turned_on[led_number] = true;
 }
}

void LEDController::turn_off_led(uint8_t led_number){
  if (this->led_number_is_valid(led_number)){
	led_is_turned_on[led_number] = false;
  }
}


void LEDController::update_display_to_next_charlieplexing_frame(){	
	turn_off_all_charlieplexed_leds();
        if (led_is_turned_on[current_led_being_shown]){
		turn_on_charlieplexed_led(current_led_being_shown);
	}
	current_led_being_shown = (current_led_being_shown+1) % NUMBER_OF_CHARLIEPLEXED_LEDS;
}
