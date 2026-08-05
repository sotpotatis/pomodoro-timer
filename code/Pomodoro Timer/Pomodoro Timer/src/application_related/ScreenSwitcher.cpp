#include "../../include/application_related/ScreenSwitcher.hpp"

ScreenSwitcher::ScreenSwitcher(): current_screen(nullptr) {};

void ScreenSwitcher::switch_screen(Screen& new_screen){
	if (this->current_screen != nullptr){
		this->current_screen->on_transition_from();
	}
	new_screen.on_transition_to();
	this->current_screen = &new_screen;
}
