#pragma once
#include "Screen.hpp"

// Represents a state machine that handles switching between the different states ("UI Screens")
// of the Pomodoro Timer UI. It is analogous to the "Context" in the GoF "State" design pattern:
// https://refactoring.guru/design-patterns/state
class ScreenSwitcher {
	private:
		Screen* current_screen;
	public:
                // Constructor sets current_screen to nullptr. Use switch_screen after
                // constructing to set the desired initial screen.
                ScreenSwitcher();
		
                void switch_screen(Screen& new_screen);
};
