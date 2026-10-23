#include <avr/interrupt.h>
#include "../../include/application_related/Screens.hpp"
#include "../../include/application_related/ScreenSwitcher.hpp"
#include "../../include/hardware_related/leds/LEDController.hpp"


Timer timer;
	
ISR(TIM0_COMPA_vect) {
	timer.tick();
}

int main(){
	LEDController led_controller;
	ScreenSwitcher screen_switcher;
	Screens screens {led_controller, screen_switcher, timer};
	timer.initialize_hardware();
	screen_switcher.switch_screen(screens.startup_screen);
	while (true){
		timer.handle_new_events();
	}
}
