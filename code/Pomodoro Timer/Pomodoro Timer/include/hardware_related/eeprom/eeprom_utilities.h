/* eeprom_utilities.h
Header declaring functions that can be found in the ASM file
eeprom_utilities.S.
*/
#include <stdint.h>
#ifndef EEPROM_UTILITIES_H_
	#define EEPROM_UTILITIES_H_
		extern "C" {
			void write_eeprom(uint16_t address, uint8_t data);
			uint8_t read_eeprom(uint16_t address);
		}
#endif