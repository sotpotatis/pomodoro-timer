#pragma once
#include "../Screen.hpp"
#include "../../hardware_related/leds/LEDController.hpp"
#include "../../hardware_related/timer/Timer.hpp"
#include "../../hardware_related/timer/TimerNotifiable.hpp"
#include "../ScreenSwitcher.hpp"

class Screens;
class StartupScreen: public Screen, TimerNotifiable {
private:
  LEDController& led_controller;
  ScreenSwitcher& screen_switcher;
  Timer& timer;
  Screens& screens;
public:
  StartupScreen(LEDController& led_controller, ScreenSwitcher& screen_switcher, Timer& timer, Screens& screens);
  void on_transition_to() override;
  void on_transition_from() override;
  void notify_about_timeout() override;
};
