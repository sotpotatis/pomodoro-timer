#include "../../../include/application_related/screens/StartupScreen.hpp"

StartupScreen::StartupScreen(LEDController& led_controller, ScreenSwitcher& screen_switcher, Timer& timer, Screens& screens) : led_controller(led_controller), screen_switcher(screen_switcher), timer(timer), screens(screens) {};

void StartupScreen::on_transition_to(){
  this->led_controller.turn_on_led(0);
  this->led_controller.update_display_to_next_charlieplexing_frame();
  this->timer.subscribe_to_periodic_callbacks(*this, 1000);
}

void StartupScreen::on_transition_from(){
  // TODO
}

void StartupScreen::notify_about_timeout(){
	
}