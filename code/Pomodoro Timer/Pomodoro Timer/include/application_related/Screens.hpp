#pragma once
#include "./screens/StartupScreen.hpp"
#include "../hardware_related/leds/LEDController.hpp"
#include "ScreenSwitcher.hpp"
#include "Screen.hpp"

class Screens {
	public:
	StartupScreen startup_screen;
	// Constructs all screens used in the application. The arguments are dependencies that are required by
	// the constructors of the application screen classes.
	Screens(LEDController& led_controller, ScreenSwitcher& screen_switcher);
};