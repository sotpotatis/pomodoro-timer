/* hardwareSetup.h
Header declaring functions that can be found in the ASM file
hardwareSetup.S
*/


#ifndef HARDWARESETUP_H_
#define HARDWARESETUP_H_
extern void setPinStates(unsigned short mask, unsigned char targetValue, unsigned char operationType);
extern void setInitialPinStates();
#endif