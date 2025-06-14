/* eepromUtilities.h
Header declaring functions that can be found in the ASM file
eepromUtilities.S.
*/
#include <stdint.h>
#ifndef EEPROMUTILITIES_H_
	#define EEPROMUTILITIES_H_
		extern void writeEEPROM(uint16_t address, uint8_t data);
		extern uint8_t readEEPROM(uint16_t address);
#endif