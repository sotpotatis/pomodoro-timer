#pragma once

#include "../hardware_related/leds/led_controller.hpp"

class ScreenController {};

class System {
private:
  LEDController ledController;
  ScreenController screenController;
public:
  System();
};
