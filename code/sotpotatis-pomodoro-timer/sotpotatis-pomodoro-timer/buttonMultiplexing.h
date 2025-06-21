/* buttonMultiplexing.h
Header declaring functions that can be found in the ASM file
buttonMultiplexing.S.
*/
#include <stdint.h>
#ifndef BUTTONMULTIPLEXING_H_
	#define BUTTONMULTIPLEXING_H_
		extern void updateButtonStates(uint8_t* buttonCounts, uint8_t* debouncedCounts, uint8_t* buttonDebounced,  uint8_t* buttonTapped, uint8_t* buttonHeld, uint8_t currentADCButton);
		extern uint8_t getCurrentADCButton(uint8_t adcSample);
#endif