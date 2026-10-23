#pragma once

// Interface for declaring that an object can be notified (at a set time interval) by the timer
// implemented in Timer.hpp. "Be notified"  implies that one object function is being called.
class TimerNotifiable {
	public:
		virtual void notify_about_timeout() = 0;
		
		// Note: This will also disable moving (see Screen.hpp)
		TimerNotifiable(const TimerNotifiable&) = delete;
		TimerNotifiable& operator=(const TimerNotifiable&) = delete;
	
	protected:
		TimerNotifiable() = default;
		// Disables destruction of Screen derivatives through the base pointer
		~TimerNotifiable() = default;
};