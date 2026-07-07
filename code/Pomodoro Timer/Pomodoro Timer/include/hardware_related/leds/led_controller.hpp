#pragma once

#include <stdint.h>

class LEDController {
private:
  void turn_on_led(uint8_t led_number);
  void turn_off_all_leds();
};
