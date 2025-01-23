/* hardwareConst.h
Defines the interface between C and ASM for hardwareSetup.S.
That is, defines C templates for everything in hardwareSetup.S.
*/


#ifndef HARDWARECONST_H_
#define HARDWARECONST_H_
extern void enableInputPinInterrupts();
extern void setPinStates(unsigned short mask, unsigned char targetValue, unsigned char operationType);
extern void setInitialPinStates();
#endif