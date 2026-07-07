/*
 *  main.c
 * Main code of the project
 */
#include "../../include/software_constants.h"
#if RUN_TIMER_CALIBRATION == 0 && RUN_TIMER_TEST == 0
	#include "../../include/hardware_constants.h"


	#include "../../include/hardware_related/leds/led_handlers.h"

	#include "../../include/hardware_related/adc/adc_utilities.h"
	
	#include "../../include/hardware_related/pins/pin_utilities.h"

	#include "../../include/hardware_related/timer/timer_utilities.h"

	#include <stdint.h>

	#include <avr/interrupt.h>

	#include "../../include/hardware_related/eeprom/eeprom_utilities.h"

	#include "../../include/hardware_related/buttons/button_multiplexing.h"

	// Global variables
	volatile uint32_t timestamp = 0;
	volatile uint8_t latest_adc_sample = 0; // The latest read sample
	volatile uint8_t latest_adc_sample_checked = 1; // Whether the latest ADC sample was acknowledged by main or not.
	volatile uint8_t latest_timer_tick_acknowledged = 1; // Flag that main can use to ack timer ticks

	/*
	Defines a custom ISR for ADC readings.
	*/
	ISR(TIM0_COMPA_vect) {
	  timestamp++;
	  latest_timer_tick_acknowledged = 0;
	  if (timestamp % ADC_SAMPLE_RATE == 0 && latest_adc_sample_checked) {
		clear_adc_interrupts();
		latest_adc_sample = read_current_adc_value();
		latest_adc_sample_checked = 0;
	  }
	}



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

	int main(void) {
	  // Set up ADC on chosen pin, in Free Running Mode, with no Interrupts
	  // and the chosen prescaler
	  set_up_adc(BUTTON_ADC_PIN, 0, 1, 0, ADC_PRESCALER_VALUE);
	  // Set up a timer in CTC mode, and generate interrupts
	  // every 1ms. Enable output compare pin on OC0A pin (PB2)
	  set_pin_states(4, 1, 0);
	  set_up_timer_in_ctc_mode(1, TICKS_PER_MS, TIMER_PRESCALER_VALUE, 1);
	  // Ensure all pins used for LED Charlieplexing are inputs.
	  reset_all_charlieplexing_pins();
	  sei();
	  uint8_t current_index = 0;
	  uint8_t blink = 0;
	  uint8_t current_state = 0; // 0 = work, 1 = rest, 2 = repetitions
	  int8_t current_turned_off_led = -1;
	  uint16_t melody_time_index = 0;
	  uint8_t melody_pause_index = 0;
	  uint8_t melody_playing = 1;
	  // Proof of concept code:
	  // lights up all 12 LEDs. If the user clicks the 5 button-panel,
	  // they should see the LED which corresponds to the button index that they just clicked
	  // turn off.
	  while (1) {
		// Set Charlieplexing LEDs to their current state.
		if (!latest_timer_tick_acknowledged) {
			if (timestamp % 1000 == 0){
				// Blink the last LED on the panel every second
				blink = !blink;
			}
			// Turn on LEDs. Lower 12 are used for the ADC multiplexing demo. Upper 3 are used for the work / rest / repetition set
			// screen
			if (current_index < 12){
				if (current_turned_off_led != current_index && (current_index != 11 || !blink)){set_charlieplexing_state(current_index);}
				current_index++;
			}
			else  {
				set_charlieplexing_state(12+current_state);
				current_index = 0;
			}
			
			// Update the buzzer.
			// The buzzer plays a melody which is tracked using "melody_index". Just temporarily for testing.
			if (melody_playing){
				melody_time_index++;
				if (melody_time_index == 200){
					melody_time_index = 0;
					melody_playing = 0;
					// Disable OCR0A to stop buzzer from sounding. 
					// Can be done in two ways - either by writing
					// to the timer register or by simply setting the pin to an input.
					// NOTE: I commented out the timer register writing method.
					// It caused a humming noise when the timer was not connected during my testing setup.
					set_pin_states(4, 0, 0); 
					// set_output_compare_pin_a(0);
				}
			}
			else {
				if ((melody_pause_index  == 3 && melody_time_index < 999) || (melody_pause_index < 3 && melody_time_index < 100)){
					melody_time_index++;
				}
				else {
					melody_time_index = 0;
					melody_playing = 1;
					if (melody_pause_index < 3){
						melody_pause_index++;
					}
					else {
						melody_pause_index = 0;
					}
					// Reenable buzzer sound. Buzzer is connected to OCR0A pin.
					// Can be done in two ways - either by writing
					// to the timer register or by simply setting the pin to an output.
					// NOTE: I commented out the timer register writing method.
					// It caused a humming noise when the timer was not connected during my testing setup.
					set_pin_states(4, 1, 0); // Set OCR0A to an output again
					// set_output_compare_pin_a(1);
				}
			}
			latest_timer_tick_acknowledged = 1;
			// Process any new ADC samples to detect button preses
			if (!latest_adc_sample_checked) {
				// No need to use a local copy of latest_adc_sample here as the ISR only updates it
				// when latest_adc_sample_checked=1.
			 uint8_t current_adc_button = get_current_adc_button(latest_adc_sample);
			update_button_states(button_counts, debounced_counts, button_debounced, button_tapped, button_held, current_adc_button);
			  // Visually show on my LEDs what button that was detected as pressed
			  for (int i = 0; i < 5; i++) {
				if (button_held[i] || button_tapped[i]) {
				  button_held[i] = 0;
				  button_tapped[i] = 0;
				  current_turned_off_led = i;
				  // Update the current state, for illustrative purposes
					current_state++;
					if (current_state == 3){
						current_state = 0;
					}
				}
			  }
			  latest_adc_sample_checked = 1;
			}
		}
		
	  }
	}
#endif