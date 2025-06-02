/* adcUtilities.S
 Contains code to use the analog inputs on the device. */
#include <avr/io.h>
#define __SFR_OFFSET        0 // Need to redefine this for proper I/O address referencing

.global setUpADC, ADC_TRIGGER_SOURCE_FREE_RUNNING_MODE, clearADCInterrupts, waitForInterrupts, readCurrentADCValue
.data   
    .align 2
    // Helper constants for setUpADC trigger source argument
    .equ ADC_TRIGGER_SOURCE_FREE_RUNNING_MODE, 0
.text
    /*
    Sets up ADC, using the internal voltage reference.
    The function defines the following input arguments:
    r25: Unused
    r24: The ADC pin to use. A number 0-7.
    r23: Unused
    r22: How to trigger ADC readings. One of these:
    - value 0: use Free Running Mode (see datasheet)
    (this parameter is defined for futureproofing and only has one current value)
    r21: Unused
    r20: Whether the ADC should read left adjusted results or not. (1 == left adjust, 0 == do not left adjust)
	r19: Unused
	r18: Whether to enable ADC interrupts or not. (1 == enable interrupts, 0 == do not enable interrupts)
	Note that for interrupts to work, global interrupts must also be enabled (the I-bit in SREG must be set)
	r17: Unused
	r16: The ADC prescaler selection value, i.e. controls the relationship between how often (in fractions of clock cycles) to read the ADC.
	A 3-bit value- i.e. 111 for a 1/128 ADC prescaler division factor.
    */
    setUpADC:
		// Mask inputs
		andi r24, 0x7
		andi r22, 0x1
		andi r20, 0x1
		andi r18, 0x1
		andi r16, 0x7
        // Note that quotes are from the datasheet
        // Ensure PRADC = 0: "Writing a logic one to this bit shuts down the ADC"
        cbi PRR, 0
        // Enable ADC by setting ADCEN = 1 "Writing this bit to one enables the ADC"
        sbi ADCSRA, 7
		// Check if interrupts should be enabled
		// Interrupts are controlled using the bit 3 in ADSCRA.
		cpi r18, 0
		breq noInterrupts
		sbi ADCSRA, 3
		rjmp setVoltageReference
		noInterrupts:
			cbi ADCSRA, 3
		setVoltageReference:
			// Set the internal voltage reference by clearing REFS[1:0] bits in ADMUX
			// (which select the voltage reference)
			in r19, ADMUX
			andi r19, 0x3F
			// Set the ADC auto trigger source, i.e. when new readings are triggered
			// These operations all work with ADTS bits, which are
			// the lower 3 bits of the ADCSRB register.
			// Read ADCSRB into r18
			in r18, ADCSRB 
			// Check if the result should be left adjusted or not
			cpi r20, 0
			breq noLeftAdjust
			// If left adjust, we should set ADLAR (bit 4 in ADCSRB) to 1.
			// Else it should be 0.
			ori r18, 0x10
			rjmp checkADCAutoTriggerSource
			noLeftAdjust:
				andi r18, 0xEF
			checkADCAutoTriggerSource:
				// Check what auto trigger shource that the user has selected
				// 0 = free running mode
				cpi r22, 0
				breq freeRunningMode
				// else: no auto trigger source selected
				rjmp setADCAutoTriggerSource
				freeRunningMode:
					andi r18, 0xF8 // Free running mode has all the ADTS bits set to 0.
					rjmp setADCAutoTriggerSource
				setADCAutoTriggerSource:
					out ADCSRB, r18
			// --- Done setting ADC auto trigger source --
			// Ensure the requested ADC pin is set to an input pin
			PUSH r24
			PUSH r19
			PUSH r16
			// Call setPinStates with according to its arguments (see its docstring):
			/* r25=<mask of PA pins to set to inputs>
			r24=0 (as we do not want to modify any B pin)
			r22=0 to mark setting as inputs.
			r20=0 to mark that we want to change the pin direction (the DDRA/B register)
			*/
			ldi r25, 1
			// left shift the 1 as many times as its pin number. This is because this function accepts
			// a pin number (0-7) while the setPinStates function use a pin mask.
			// This is not very clean, however, it has to be done, as the ADMUX has one way of providing the
			// pin number to use (this is what this function tries to mimic), and the DDRX/PORTX registers have another.
			untilFullyShifted:
				lsl r25
				dec r24
				brne untilFullyShifted

			// See comment above for these three settings
			ldi r22, 0
			ldi r20, 0
			rcall setPinStates
			POP r16
			POP r19
			POP r24
			// --- Done ensuring ADC pin is input --
			// Set the ADC channel that is to be used by clearing
			// the MUX0-5 bits in ADMUX, then OR:ing with the wanted pin to read ADC values from.
			andi r19, 0xC0 // Clear bits 5-0 (MUX5-MUX0)
			or r19, r24 // Set the pin to use
			// Write the new ADMUX values
			out ADMUX, r19
			// 
			// Now set the ADC prescaler value (the lower 3 bits of ADCSRA)
			in r19, ADCSRA
			andi r19, 0xF8
			or r19, r16
			// Finally, we want to write the ADATE bit to 1 to actually enable ADC auto triggering
			// ...aaaand set the ADSC bit to start the ADC conversion (ADSC=ADCStartConversion)
			// These are bits 5 and 6 in the ADSCRA register respectively.
			ori r19, 0x60
			out ADCSRA, r19
			ret

	/* Clears any ADC interrupt, i.e. acknowledges the interrupt flag for ADC interrupts. */
	clearADCInterrupts:
		sbi ADCSRA, 4
		ret

	/* Reads the ADC ADCH data register and returns the read value in r24. 
	NOTE: This function only works if ADLAR = 1, i.e. if the ADC result is set to be left adjusted.
	If you have ADLAR = 0, then you need another function which reads ADCL before ADCH.*/
	readCurrentADCValue:
		in r24, ADCH
		ret

  /* Waits for interrupts from the processor. */
   waitForInterrupts:
		sleep
		ret