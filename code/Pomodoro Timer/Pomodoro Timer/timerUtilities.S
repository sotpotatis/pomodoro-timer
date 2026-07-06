
/*
 * timerUtilities.S
Contains utilitiyy functions to interface with the Attiny84A timer as used with this project.
 */
 #include "hardwareConst.h"
 #include <avr/io.h>

 .global setUpTimerInCTCMode, setOutputComparePinA

 .text
 /*
 Utility function to set up the timer in CTC mode. Uses Output Compare Match A.
 Takes the following arguments:
 r25: Unused.
 r24: 1,2 or 3 to enable the output compare pins (see datasheet Table 11-2). 0 to disable them.
 r23: Unused.
 r22: The value to count to, i.e. the value to set Output Compare Match A to. The timer will roll back to 0 after this value.
 r21: Unused.
 r20: The clock source to use as determined by the CS0[2:0]: Clock Select bits.
 r19: Unused.
 r18: Whether to enable interrupts from the timer or not. More specifically, output compare match A interrupts.
 */
 setUpTimerInCTCMode:
	// Mask inputs
	andi r20, 0x07
	andi r18, 0x01
	// Define value to count to in OCR0A.
	out OCR0A, r22
	PUSH r20
	PUSH r18
	rcall setOutputComparePinA
	POP r18
	POP r20
	// Set the clock source and prescaler 
	// by writing what the user chose to configurate
	// to TCCR0B bits 2:0.
	// Also, clear bit 3 while we're at it - Bit 3 is WGM02 which should be set to 0
	// when we are using the CTC mode
	in r24, TCCR0B
	andi r24, 0xF0
	// Do an or operation to get the user's defined clock selection arguments
	// in there
	or r24, r20
	out TCCR0B, r24
	// Set OCIE0A (bit 1 in the TIMSK0 register) to enable timer output compare match interrupts.
	// If the user has selected that they want to do so
	in r24, TIMSK0
	cpi r18, 1
	brne noInterrupts
	ori r24, 0x02
	rjmp writeNewTIMSKValue
	noInterrupts:
	andi r24, 0xFD
	writeNewTIMSKValue:
	out TIMSK0, r24
	ret
/*
Function to set output compare pin A to enabled or disabled.
NOTE: This function will also enable CTC mode if not already enabled.
NOTE: The pins must be set as outputs in another part of the code in order to work.
r24: 1,2 or 3 to enable the output compare pins (see datasheet). 0 to disable them.
r23: Unused
*/
setOutputComparePinA:
	// Mask inputs
	andi r24, 0x3
	in r22, TCCR0A
	// Set WGM0[1:0] (bit 1:0 in TCCR0A) to 0b10 to enable CTC mode
	andi r22, 0x3C // Clear WGM0[1:0] and the output compare pin A bits (COM0A[1:0], bit 7-6 in TCCR0A)
	ori r22, 0x02 // Set WGM[1:0]
	cpi r24, 0
	breq writeNewTCCR0AValues // If we get here, the user wants COM0A[1:0] to be cleared, but that would have already happened above
	// COM0A[1:0] is bit 7-6 in TCCR0A, so prepare the value we will be OR:ing with.
	swap r24
	lsl r24
	lsl r24
	or r22, r24
	writeNewTCCR0AValues:
		out TCCR0A, r22
	ret