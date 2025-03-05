/* ledHandlers.S
Handles the updating of the LED display to show a certain
*/
#include <avr/io.h>
#define __SFR_OFFSET        0 // Need to redefine this for proper I/O address referencing
.global setCharlieplexingState
.data
	.align 2
.text
    // The states of the Charlieplexing pins. The first index in this sequence of bytes represent the pin state
    // if you want to address LED0, the second index LED1, and so on and so forth.
    // Each state is represented as 8 bits packed as: <4 unused bits>L1L0|H1H0 where H1H0 represents a binary number 0-3 of the pin
    // to set as input. Similarly, L1L0 represents the pin to set as output. As an example: 1101 means: pin 3 (11 base 2)
    // should be set to high output and pin 1 (01 base 2) should be set to output.
    // The remaining pin for each states are set as outputs.
    CHARLIEPLEXING_PIN_STATES: .byte 0x10, 0x01, 0x20, 0x02, 0x30, 0x03, 0x21, 0x12, 0x31, 0x13, 0x32, 0x23

    /**
    Given Charlieplexing pin i (0-3), returns the pin mask of i.
    (the constant that needs to be loaded and provided to the setPinStates function to manipulate the status
    of Charlieplexing pin i)
    r25: Unused.
    r24: The pin number of the Charliplexing pin. 0-3.
    Returned pin mask is stored in r25-r24. (MSb's in r25, LSb's in r24)
    */
    loadPinMask:
        cpi r24, 1
        breq pin1
        cpi r24, 2
        breq pin2
        cpi r24, 3
        breq pin3
        pin0:
            ldi r25, hi8(LED_CHARLIEPLEXING_PIN_1)
            ldi r24, lo8(LED_CHARLIEPLEXING_PIN_1)
            rjmp return
        pin1:
            ldi r25, hi8(LED_CHARLIEPLEXING_PIN_2)
            ldi r24, lo8(LED_CHARLIEPLEXING_PIN_2)
            rjmp return
        pin2:
            ldi r25, hi8(LED_CHARLIEPLEXING_PIN_3)
            ldi r24, lo8(LED_CHARLIEPLEXING_PIN_3)
            rjmp return
        pin3:
            ldi r25, hi8(LED_CHARLIEPLEXING_PIN_4)
            ldi r24, lo8(LED_CHARLIEPLEXING_PIN_4)
        return:
            ret
    /**
    Sets all Charlieplexing pins to inputs.
    Takes no input arguments.
    */
    setAllCharlieplexingPinsToInputs:
        // Our main strategy:
        // call setPinStates
        // according to the calling conventions of setPinStates: 
        // r25-r24: pin masks from LED_CHARLIEPLEXING_STATE_1, LED_CHARLIEPLEXING_STATE_2 etc.
        // r20: =0, which means set pins to outputs/inputs
        // r22: =0, signalling setting pins to inputs
        //
        // for each pin number
        ldi r24, 3
        for_setAllCharlieplexingPinsToInputs:
            // Load different values (pin masks) into r24 and r25 depending on pin number
            PUSH r24 // push index counter to stack
            rcall loadPinMask // loadPinMask will fill out r24 and r25 with our wanted pin mask;
			// see the "Our main strategy comment above"
            // Load constant values that are needed to call the function
            // See the comment above
            ldi r22, 0
            ldi r20, 0
            rcall setPinStates
            POP r24 // pop index counter from stack
            // until this loop has been run 4 times
            dec r24
            brge for_setAllCharlieplexingPinsToInputs
        ret
    /**
    Sets the microcontroller pins to appropriately address a certain output pin via the method Charlieplexing.
    r25-r24: The pin to set to output as a number 0-11. For example, to controll LED0, input 0 to the function. To control LED3, input
    3 (base 10). Lower 4 bits in r24, rest unused.
    */
    setCharlieplexingState:
        // Mask inputs
        andi r24, 0xf
        // Calculate the address to load CHARLIEPLEXING_PIN_STATES, which is located in program memory,
        // from.
        // The byte offset to load from is the value of r25-r24 (the input args to our funtion). For instance, to load byte 0 in the sequence of
        // CHARLIEPLEXING_PIN_STATES, 0 is provided as input to the function.
        ldi ZL, lo8(CHARLIEPLEXING_PIN_STATES)
        ldi ZH, hi8(CHARLIEPLEXING_PIN_STATES)
        add ZL, r24
        adc ZH, r1 // Add any carry from ZL + r24 into ZH
        lpm r23, Z // Load desired pin state into r23
        // (1) set all pins to inputs
        PUSH r23
        rcall setAllCharlieplexingPinsToInputs
        POP r23
        // r19 = <pin number to set to output low>
        mov r19, r23
        andi r19, 0xf0
        swap r19 // swaps the high and low nibble of r19,
        // so that r19 is 0x02 and not 0x20, as an example
        // r18 = <pin number to set to output high>
        mov r18, r23
        andi r18, 0x0f
        // Set the desired state for each of the 4 Charliplexing pins
        // (2) Call setPinStates(<output pin mask>, 1, 0) for each of the output pins,
        // to set them to outputs
        // This is done by setting: r25-r24=requested pin mask, r22=1, r20=0
		ldi r24, 3
        for_setCharlieplexingState: // For all pins 0 - 3
            cp r24, r19
            breq setPinsToOutput
            cp r24, r18
            breq setPinsToOutput
            rjmp next // If pin is not an output pin
            // Perform register operations to load arguments
            // according to the calling conventions of setPinStates: 
            // r25-r24: pin masks from LED_CHARLIEPLEXING_STATE_1, LED_CHARLIEPLEXING_STATE_2 etc.
            // r20: =1, which means set pin state
            // r22: =1/0, the value to set the pin to
            setPinsToOutput:
                // Save states of: pin number to set to low, pin number to set to high, index counter.
                PUSH r19
                PUSH r18
                PUSH r24
                // Call functions needed to alter pin state
                // First change the pin mode (set pin to output).
                // This is done by calling setPinStates(r25-24=pin mask, r20=0 (set pin mode), r22=1 (set pins to output))
                ldi r20, 0
                ldi r22, 1
                rcall loadPinMask // Loads the pin mask of the Charlieplexing pin into r25-24
                rcall setPinStates // Sets pin to have the pin mode output
                // Recover registers previsiouly pushed
                POP r24
                POP r18
                POP r19
            next: // Proceed to next iteration
                dec r24
                brge for_setCharlieplexingState
        // Set the Charlieplexing output high pin to high
        // by calling setPinStates(r25-24=pin mask, r20=1 (set pin value), r22=1 (set pins to output))
        cp r24, r18
        rcall loadPinMask // Loads the pin mask of the Charlieplexing pin into r25-24
        ldi r20, 1
        ldi r22, 1
        rcall setPinStates // Sets pin to have the pin mode output
        ret