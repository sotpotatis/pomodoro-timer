
/*
 * timerUtilities.S
Contains utilitiyy functions to interface with the Attiny84A timer as used with this project.
 */
 #include "hardwareConst.h"
 #include <avr/io.h>

 .global setUpTimerInCTCMode

 .text
 /*
 Utility function to set up the timer in CTC mode.
 Takes the following arguments:
 r25: Unused.
 r24: The value to count to, i.e. the value to set Output Compare Match A to. The timer will roll back to 0 after this value.
 r23: Unused.
 r22: Whether to disable the compare pins or not. If you need to use the compare pins for some reason, set them beforehand and then use this argument
 to avoid over-writing.
 If r22=0, it doesn't mess with the compare pins, leaving whatever bits were set there previously. If r22=1, both the compare match A and B pins are
 disabled.
 r21: Unused.
 r20: The clock source to use as determined by the CS0[2:0]: Clock Select bits.
 r19: Unused.
 r18: Whether to enable interrupts from the timer or not. More specifically, output compare match A interrupts.
 */
 setUpTimerInCTCMode:
	// Mask inputs
	andi r22, 0x01
	andi r20, 0x07
	andi r18, 0x01
	// Define value to count to in OCR0A.
	out OCR0A, r24
	// Start manipulating the TCCR0A values
	in r24, TCCR0A
	// Set WGM0[1:0] (bit 1:0 in TCCR0A) to 0b10 to enable CTC mode
	andi r24, 0xFC // Clear WGM0[1:0]
	ori r24, 0x02
	// IF the user has not set r22=0
	// Set COM0A[1:0] AND COM0B[1:0] (bit 7,6,5,4 in TCCR0A) to disable the output compare pins.
	// (i.e. clear all of these bits)
	cpi r22, 1
	brne writeNewTCCR0AValues
	andi r24, 0x0F // Clear bit 7-4 of TCCR0A, see above for more information :)
	writeNewTCCR0AValues:
		out TCCR0A, r24
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