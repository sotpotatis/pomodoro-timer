#pragma once

// An interface that should be implemented by classes containing the behavior of a state
// of the Pomodoro Timer UI. The state classes can then be used with the state machine
// contained in ScreenSwitcher.hpp.
// Analogous to the "State" *interface* in the GoF "State" *design pattern*:
// https://refactoring.guru/design-patterns/state
class Screen {
	public:
		virtual void on_transition_to() = 0;
		virtual void on_transition_from() = 0;
		
	
		// Note: This will also disable moving (see C++ Primer by Lippman et al., 5th ed,
		// p. 624)
		Screen(const Screen&) = delete;
		Screen& operator=(const Screen&) = delete;
	
	protected:
		Screen() = default;
		// A protected destructor disables destruction of Screen derivatives through the 
		// base pointer, e.g.
		// Screen * s = ...;
		// delete s;
		~Screen() = default;
		
};