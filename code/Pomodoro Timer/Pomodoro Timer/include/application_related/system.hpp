#pragma once

#include "../hardware_related/leds/LEDController.hpp"
#include "ScreenSwitcher.hpp"

class System {
private:
LEDController led_controller;
ScreenSwitcher screen_controller;
public:
System();
LEDController& get_led_controller();
ScreenSwitcher& get_screen_controller();
};
