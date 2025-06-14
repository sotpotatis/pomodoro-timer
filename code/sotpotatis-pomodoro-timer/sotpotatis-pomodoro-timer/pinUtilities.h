/* pinUtilities.h
Header declaring functions that can be found in the ASM file
pinUtilities.S
*/

#include <stdint.h>
#ifndef PINUTILITIES_H_
#define PINUTILITIES_H_
extern void setPinStates(uint16_t mask, uint8_t targetValue, uint8_t operationType);
#endif