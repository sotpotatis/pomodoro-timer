/* timer_utilities.h
Header declaring functions that can be found in the ASM file
timer_utilities.S.
*/

#ifndef TIMER_UTILITIES_H_
	#define TIMER_UTILITIES_H_
		extern "C" {
			void set_up_timer_in_ctc_mode(uint8_t output_compare_pin_settings, uint8_t value_to_count_to, uint8_t clock_source, uint8_t enable_interrupts);
			void set_output_compare_pin_a(uint8_t output_compare_pin_settings);
		}
#endif