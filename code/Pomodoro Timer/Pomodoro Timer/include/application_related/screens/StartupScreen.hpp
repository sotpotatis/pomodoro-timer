#pragma once
#include "../Screen.hpp"
#include "../../hardware_related/leds/LEDController.hpp"
#include "../ScreenSwitcher.hpp"

class Screens;
class StartupScreen: public Screen {
private:
  LEDController& led_controller;
  ScreenSwitcher& screen_switcher;
  Screens& screens;
public:
  StartupScreen(LEDController& led_controller, ScreenSwitcher& screen_switcher, Screens& screens);
  void on_transition_to() override;
  void on_transition_from() override;
};
