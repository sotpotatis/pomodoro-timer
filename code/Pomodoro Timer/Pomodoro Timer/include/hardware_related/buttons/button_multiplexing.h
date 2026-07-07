/* button_multiplexing.h
Header declaring functions that can be found in the ASM file
button_multiplexing.S.
*/
#include <stdint.h>
#ifndef BUTTON_MULTIPLEXING_H_
	#define BUTTON_MULTIPLEXING_H_
		extern "C" {
			void update_button_states(uint8_t* button_counts, uint8_t* debounced_counts, uint8_t* button_debounced,  uint8_t* button_tapped, uint8_t* button_held, uint8_t current_adc_button);
			uint8_t get_current_adc_button(uint8_t adc_sample);
		}
#endif