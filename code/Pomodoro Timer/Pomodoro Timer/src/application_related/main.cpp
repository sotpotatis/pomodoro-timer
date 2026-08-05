#include <avr/interrupt.h>
#include "../../include/application_related/Screens.hpp"
#include "../../include/application_related/ScreenSwitcher.hpp"
#include "../../include/hardware_related/leds/LEDController.hpp"

ISR(TIM0_COMPA_vect) {
// TODO
}

int main(){
	LEDController led_controller;
	ScreenSwitcher screen_switcher;
	Screens screens {led_controller, screen_switcher};
	screen_switcher.switch_screen(screens.startup_screen);
}
