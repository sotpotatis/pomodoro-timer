/* pin_utilities.h
Header declaring functions that can be found in the ASM file
pin_utilities.S
*/

#include <stdint.h>
#ifndef PIN_UTILITIES_H_
	#define PIN_UTILITIES_H_
		extern "C" {
			void set_pin_states(uint16_t mask, uint8_t target_value, uint8_t operation_type);
		}
#endif