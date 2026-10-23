#include "TimerNotifiable.hpp"
#include <stdint.h>
#include "../../software_constants.h"

#pragma once


const uint8_t MAX_NUMBER_OF_TIMER_SUBSCRIPTIONS = 10;

struct TimerSubscription {
	TimerNotifiable * subscribing_object;
	uint16_t interval_ms;
	uint16_t ms_remaining_until_next_notification;
	
	// Default constructing gives an equivalent to an "unused subscription slot".
	TimerSubscription() 
	: subscribing_object(nullptr)
	, interval_ms(0)
	, ms_remaining_until_next_notification(0) {};
	
	TimerSubscription(TimerNotifiable * subscribing_object, uint16_t interval_ms, uint16_t ms_remaining_until_next_notification)
	: subscribing_object(subscribing_object)
	, interval_ms(interval_ms)
	, ms_remaining_until_next_notification(ms_remaining_until_next_notification) {};
};

class Timer {
	public:
		void tick();
		void handle_new_events();
		void initialize_hardware();
		void subscribe_to_periodic_callbacks(TimerNotifiable& object_to_subscribe, uint16_t interval_ms);
		void unsubscribe_from_periodic_callbacks(const TimerNotifiable& object_to_unsubscribe);
		Timer();
	private:
		int find_subscription_index_for(const TimerNotifiable& subscribing_object);
		int number_of_timer_subscriptions;
		volatile bool has_unacknowledged_tick; // volatile because this is used in an ISR routine
		TimerSubscription timer_subscriptions[MAX_NUMBER_OF_TIMER_SUBSCRIPTIONS];
};