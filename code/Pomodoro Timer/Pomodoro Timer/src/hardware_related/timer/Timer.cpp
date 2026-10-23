#include "../../../include/hardware_related/timer/Timer.hpp"
#include "../../../include/hardware_related/timer/TimerNotifiable.hpp"
#include "../../../include/hardware_related/timer/timer_utilities.h"

Timer::Timer(): number_of_timer_subscriptions(0), has_unacknowledged_tick(false) {};

void Timer::tick(){
	this->has_unacknowledged_tick = true;	
};

void Timer::handle_new_events(){
	if (this->has_unacknowledged_tick){
		this->has_unacknowledged_tick = false;
		for (int i=0; i < this->number_of_timer_subscriptions; i++){
			TimerSubscription& subscription = this->timer_subscriptions[i];
			if (subscription.interval_ms > 0){
				subscription.ms_remaining_until_next_notification -= 1;
			};
			if (subscription.ms_remaining_until_next_notification == 0){
				subscription.subscribing_object->notify_about_timeout();
				subscription.ms_remaining_until_next_notification = subscription.interval_ms;
			}
		}
	}	
};

// returns -1 if a subscription index is not found.
int Timer::find_subscription_index_for(const TimerNotifiable& subscribing_object){
	for (int i=0; i < this->number_of_timer_subscriptions; i++){
		if (this->timer_subscriptions[i].subscribing_object == &subscribing_object){
			return i;
		};
	}
	return -1;
};

void Timer::subscribe_to_periodic_callbacks(TimerNotifiable& object_to_subscribe, uint16_t interval_ms){
	if (number_of_timer_subscriptions < MAX_NUMBER_OF_TIMER_SUBSCRIPTIONS){
		if (find_subscription_index_for(object_to_subscribe) == -1){ // Multiple subscriptions for same callback not allowed.
			timer_subscriptions[number_of_timer_subscriptions] = {&object_to_subscribe, interval_ms, interval_ms};
			number_of_timer_subscriptions++;
		};
	}
};

void Timer::unsubscribe_from_periodic_callbacks(const TimerNotifiable& object_to_unsubscribe){
	int requested_subscription_index = find_subscription_index_for(object_to_unsubscribe);
	if (requested_subscription_index != -1){
		// Move elements to the right of the element we are just removing.
		for (int i = requested_subscription_index+1; i < this->number_of_timer_subscriptions; i++){
			this->timer_subscriptions[i-1] = this->timer_subscriptions[i];	
		};
		this->timer_subscriptions[number_of_timer_subscriptions-1] = TimerSubscription();
		this->number_of_timer_subscriptions--;
	};
};


void Timer::initialize_hardware(){
	set_up_timer_in_ctc_mode(1, TICKS_PER_MS, TIMER_PRESCALER_VALUE, 1);
};
