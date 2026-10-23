#include "../../include/application_related/Screens.hpp"

Screens::Screens(LEDController& led_controller, ScreenSwitcher& screen_switcher, Timer& timer): startup_screen(led_controller, screen_switcher, timer, *this) {}
