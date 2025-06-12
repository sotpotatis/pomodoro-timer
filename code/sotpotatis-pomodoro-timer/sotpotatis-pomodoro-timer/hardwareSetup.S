/*
hardwareSetup.S
Contains code for setting up and controlling hardware, currently
limited to I/0 pin operations.
*/
#include <avr/io.h>
#include "hardwareConst.h"
.global setPinStates, setInitialPinStates
.text
	/* This function handles two main tasks:
	1. Setting a pin to an input or output
	2. Setting the value of an output pin.
	The reason is that the register operations for these are basically the same, just with different registers
	r25-r24: The mask of pins to set to outputs/inputs. Upper 8 LSBits in r25, lower 4 LSBits in r24. 
	The format is as follows
	0bPA7|PA6....PA0|constant 0000 PB3|...PB0(for example: 0b0000 0011 0000 0000 would set PAO and PA1 to outputs.)
	Note that we assume that pins fill
	r22: If r20=0: 1 if setting to outputs, 0 if setting to inputs. If r20=1: The pin value to set the pins to. Either 1 for pin on or 0 for pin off.
	r20: 1 if you are changing the pin value (the PORTA/B register), 0 if you are changing the pin type (the DDRA/B register)
	*/
	setPinStates:
		// Ensure masking of input arguments
		andi r24, PB_PIN_MASK
		andi r25, PA_PIN_MASK
		andi r22, 1
		andi r20, 1
		// Depending on the selection of the r20 value we operate on different registers. Please see above for details.
		cpi r20, 1
		breq pinregisters_load
		ddrregisters_load:
			// Load current DDR values
			in r19, DDRA
			in r18, DDRB
			rjmp update_registers
		pinregisters_load:
			// Load current PIN values
			in r19, PORTA
			in r18, PORTB
		update_registers:
		// General procedure of setting pins to inputs/outputs:
		// Set the pins corresponding bit in DDRA or DDRB to:
		// 1 if set to output. 0 if set to input.
		// General procedure for setting output pin values:
		// Set the pins corresponding bit in PINA or PINB to the corresponding & wanted value (1 or 0)
		// If we are setting the pins to inputs or setting output values to 0, we want to clear the bits that
		// the mask specifies. Example: For instance, to set PB0 to input, the function
		// will be called with r24=0b0...1, r22=0, r20=0.
		// Following the mentioned example: At this point of the function we will have r24 = 1 and r22=0.
		// But what we want is that we want to do DDRB & 0. So we invert r24 if needed
		cpi r22, 1
		breq or_with_register
		// Invert the bits if we get here. Why is decribed above.
		com r25
		com r24
		and_with_register: // and needs to be done to set pins to inputs. Described why above
			and r18, r24 // "B" register && !B Pin mask 
			and r19, r25 // "A" register && !A Pin mask 
			rjmp write
		or_with_register: // or needs to be done to set pins to inputs. Described why above
			or r18, r24 // "B" register | B Pin mask 
			or r19, r25 // "A" register | A Pin mask 
		write:
			// Depending on the selection of the r20 value we operate on different registers. Please see above for details.
			cpi r20, 1
			breq pinregisters_write
			ddrregisters_write:
				// First, handle PA pins.
				out DDRA, r19
				// Then, handle PB pins.
				out DDRB, r18
				rjmp return
			pinregisters_write:
				// First, handle PA pins.
				out PORTA, r19
				// Then, handle PB pins.
				out PORTB, r18
		return:
			ret

	/* Initial hardware setup. 
	TODO implement this function.
	Sets up input and output pins to their desired initial states.  */
	setInitialPinStates:
		ret
