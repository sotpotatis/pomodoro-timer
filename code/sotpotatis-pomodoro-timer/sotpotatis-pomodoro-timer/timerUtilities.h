/* timerUtilities.h
Header declaring functions that can be found in the ASM file
timerUtilities.S.
*/

#ifndef TIMERUTILITIES_H_
#define TIMERUTILITIES_H_
extern void setUpTimerInCTCMode(unsigned char valueToCountTo, unsigned char disableComparePins, unsigned char clockSource, unsigned char enableInterrupts);
#endif