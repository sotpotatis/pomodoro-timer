/* adc_utilities.h
Header declaring functions that can be found in the ASM file
adc_utilities.S
*/
#include <stdint.h>
#ifndef ADC_UTILITIES_H_
	#define ADC_UTILITIES_H_
		extern "C" {	
			void set_up_adc(uint8_t adc_pin_number, uint8_t mode_to_use, uint8_t left_adjust_results, uint8_t enable_interrupts, uint8_t prescaler_selection_value);
			void clear_adc_interrupts();
			uint8_t read_current_adc_value();
		}
#endif