/* timerUtilities.h
Header declaring functions that can be found in the ASM file
timerUtilities.S.
*/

#ifndef TIMERUTILITIES_H_
#define TIMERUTILITIES_H_
extern void setUpTimerInCTCMode(uint8_t valueToCountTo, uint8_t disableComparePins, uint8_t clockSource, uint8_t enableInterrupts);
#endif