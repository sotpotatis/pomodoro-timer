/*
 * timerTest.c
 * Utility code to test the timer based on a calibrated value set by running
 * timerCalibration.c.
 * NOTE: that I am aware of this code containing some duplicated code (especially in multiplexing
 * variable setup, timer setup, ISR, and ADC). This is intended as testing code only.
 * NOTE: The timer calibration method should be credited to user "avrcandies" on AVRFreaks forums. Many thanks!
 * See this topic where it was suggested: https://www.avrfreaks.net/s/topic/a5CV40000002wfpMAA/t399784 (post 11)
 * Test is done like this: Press any button connected to the microcontroller. This starts a countdown of
 * MINUTES_TO_COUNT_TO minutes. A single LED will turn on and ack that the countdown has started. Be quick and start a stopwatch!
 * When the timer has reached MINUTES_TO_COUNT_TO minutes, all of its LEDs will turn on. Stop the stopwatch!
 * Now you can see if the timer is off time-wise or not.
 */

#include "../include/software_constants.h"
#if RUN_TIMER_TEST == 1
	#include <stdint.h>
	#include "../include/hardware_related/timer/timer_utilities.h"
	#include "../include/hardware_constants.h"
	#include "../include/hardware_related/leds/led_handlers.h"
	#include <math.h>
	#include <avr/interrupt.h>
	#include "../include/hardware_related/eeprom/eeprom_utilities.h"
	#include "../include/hardware_related/adc/adc_utilities.h"
	#include "../include/hardware_related/buttons/button_multiplexing.h"
	// Time-keeping variables
	volatile uint8_t timer_running = 0;
	volatile uint32_t timestamp = 0;
	volatile uint16_t elapsed_seconds = 0;
	uint8_t minutes_to_count_to = 63; // No. of minutes to count to for this test
    // ADC multiplexing variables
	volatile uint8_t latest_adc_sample_checked = 1;
	volatile uint8_t latest_adc_sample = 0;
	// Store a numeric counter for each button that, for each reading: counts up (+1) if the ADC detects a voltage reading mapped to that button.
	// It counts down (-1) for all buttons that the ADC are not mapped to.
	uint8_t button_counts[5] = {
		0,
		0,
		0,
		0,
		0
	};
	// Same as button_counts but for hold detection
	uint8_t debounced_counts[5] = {
		0,
		0,
		0,
		0,
		0
	};
	// Store whether buttons were debounced or not. See below for usage.
	uint8_t button_debounced[5] = {
		0,
		0,
		0,
		0,
		0
	};
	// Store whether buttons were detected as held or not.
	// These flags are cleared by main.
	uint8_t button_held[5] = {
		0,
		0,
		0,
		0,
		0
	};
	// Store whether buttons were detected as tapped or not.
	// These flags are cleared by main.
	uint8_t button_tapped[5] = {
		0,
		0,
		0,
		0,
		0
	};
	/*
	Defines a custom ISR which counts the time elapsed.
	*/
	ISR(TIM0_COMPA_vect) {
		if (timer_running == 1){
			timestamp++;
			if (timestamp % 1000 == 0){
				elapsed_seconds++;
			}
		}
		 if (timestamp % ADC_SAMPLE_RATE == 0 && latest_adc_sample_checked) {
			 clear_adc_interrupts();
			 latest_adc_sample = read_current_adc_value();
			 latest_adc_sample_checked = 0;
		 }
	}
		int main(){
			// Set up ADC and timer, with timer interrupts every 1ms
			set_up_adc(BUTTON_ADC_PIN, 0, 1, 0, ADC_PRESCALER_VALUE);
			set_up_timer_in_ctc_mode(0, TICKS_PER_MS, TIMER_PRESCALER_VALUE, 1);
			sei();
			// Calculate how many seconds we should count to, based on the timer calibration factor.
			// NOTE: The timer calibration method should be credited to user "avrcandies" on AVRFreaks forums. Many thanks!
			// See this topic where it was suggested: https://www.avrfreaks.net/s/topic/a5CV40000002wfpMAA/t399784 (post 11)
			// Calibration value is stored in EEPROM
			uint16_t calibration_value_h = read_eeprom(TIMER_CALIBRATION_EEPROM_ADDRESS_H);
			uint8_t calibration_value_l = read_eeprom(TIMER_CALIBRATION_EEPROM_ADDRESS_L);
			uint32_t calibration_value = (calibration_value_h<<8) | (calibration_value_l);
			uint16_t seconds_to_count_to = (minutes_to_count_to * calibration_value) >> 6;
			while (1){
				// Allow the user to start the timer by clicking on any button
				if (!latest_adc_sample_checked && !timer_running){
					uint8_t current_adc_button = get_current_adc_button(latest_adc_sample);
					update_button_states(button_counts, debounced_counts, button_debounced, button_tapped, button_held, current_adc_button);
					for (int i =0; i < 5; i++){
						if (button_tapped[i] || button_held[i]){
							button_tapped[i] = 0;
							button_held[i] = 0;
							timer_running = 1;
							set_charlieplexing_state(0); // Turn on LED to acknowledge countdown start
						}
					}
					latest_adc_sample_checked = 1;
				}
				// Check timeout
				if (elapsed_seconds >= seconds_to_count_to){
					// On timeout, turn on all LEDs.
					for (int i=0;i<15;i++){
						set_charlieplexing_state(i);
					}
				}
			}
		}
#endif