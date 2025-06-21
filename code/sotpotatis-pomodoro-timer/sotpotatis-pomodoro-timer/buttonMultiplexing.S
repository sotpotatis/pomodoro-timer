/* buttonMultiplexing.S
Utilties for connecting multiple buttons to one ADC pin, and
identifying which button that has been pressed. */
#include "softwareConst.h"
.global updateButtonStates, getCurrentADCButton
.text
    // Defines, for each button/circuit state, an ADC reading that if exceeded (or exactly achieved) (>=) results in that
    // state being detected (instantaneously).
    // No button, button 1, button 2, button 3, button 4, button 5
    ADC_STATES: .byte 232, 185, 129, 75, 27, 0

    /*
    Used to update the current state of buttons (if they have been clicked,
    held etc.) by operating on the five uint8_t arrays (which must have length 5)
    mentioned below:
    
    r25-r24: Pointer to a uint8_t list, buttonCounts. Stores
    a numeric counter for each button that, for each reading: counts up (+1) if the ADC detects a voltage reading mapped to that button.
    It counts down (-1) for all other buttons
    
    r23-r22: Pointer to a uint8_t list, debouncedCounts. Increases for button n if it has been declared debounced.
    
    r21-r20: Pointer to a uint8_t list, buttonDebounced. For each index, is 1 if the button has been declared debounced
    and 0 if it hasn't.
    
    r19-r18 (copied into r11-r10 by function): Pointer to a uint8_t list, buttonTapped. For each index, is 1 if the button has been declared tapped
    and 0 if it hasn't. IMPORTANT: Must be cleared by main!!! Is never cleared by this function.
    
    r17-r16 (copied into r13-r12 by function): Pointer to a uint8_t list, buttonHeld. For each index, is 1 if the button has been declared held
    and 0 if it hasn't. IMPORTANT: Must be cleared by main!!! Is never cleared by this function.

    r15: Unused

    r14: The current ADC button that was detected as pressed.
    I.e. the output from getCurrentADCButton.
	
	NOTE: This function was translated by hand from a C function, and will much likely be easier
	to understand if you look at the C function that it was translated from. See
    */
    updateButtonStates:
        // Calling conventions
        PUSH r17
        PUSH r16
        PUSH r14
        PUSH r13
        PUSH r12
        PUSH r11
        PUSH r10
        PUSH r9
        PUSH r8
		// We need to use r16 and r17 with ldi instructions so swap r17-16 (input argument *buttonHeld with r13-r12)
		mov r13, r17
		mov r12, r16
        // Same with r18 and r19
        mov r11, r19
		mov r10, r18
        ldi r17, 0 // Index counter
        ldi r16, 5 // Loop should count to 5
		ldi r19, 1 // Constant 1
        dec r14 // Decrement currentADCButton with 1,
        // as index 0 in the lists we will iterate over corresponds to
        // button 1.
        for_updateButtonStates: // For each index 0-4 in lists
            // Load buttonCounts address into Z register
            mov ZH, r25
            mov ZL, r24
            ld r9, Z // Load value buttonCounts[i]
            // Load buttonDebounced address into X register
            mov XH, r21
            mov XL, r20
			// Increase the buttonCounts variable for the button
            // which was declared as pressed.
            // Decrease the buttonCount for all other buttons.
            cp r17, r14
            brne currentButtonNotDetected
            // If current button was detected as pressed
                // If pressed: if buttonCounts[i] < DEBOUNCE_SAMPLE_LIMIT --> increase buttonCounts[i]
                // else set buttonDebounced[i] = 1
                ldi r18, DEBOUNCE_SAMPLE_LIMIT
                cp r9, r18
                brlo increaseButtonCounts
                // Else: if buttonCounts[i] >= DEBOUNCE_SAMPLE_LIMIT
                st X, r19 // Set buttonDebounced[i] = 1
                rjmp doHoldDetection
                // If buttonCounts[i] < DEBOUNCE_SAMPLE_LIMIT
                increaseButtonCounts:
                    inc r9
                    // Write back new buttonCounts value
                    st Z, r9
                    rjmp doHoldDetection
            // Else: If current button was not detected as pressed
            currentButtonNotDetected:
                // If buttonCounts[i] > 0: decrease button count
                cp r9, r1
                breq resetDebounced
                // Decrease buttonCounts[i] by 1
                dec r9
                st Z, r9
                rjmp doHoldDetection
                // Else, if buttonCounts[i] = 0, reset debounced flag
                // (i.e set buttonDebounced[i]=0)
                resetDebounced:
                    st X, r1
            doHoldDetection:
                // Perform hold detection for any button that was debounced
                ld r8, X // Load value buttonDebounced[i]
                // Load debouncedCounts[i] into Z register
                // The Z register was used for buttonCounts[i] above
                mov ZH, r23
                mov ZL, r22
                ld r9, Z // Load value debouncedCounts[i]
                cp r8, r19
                brne ifNotDebounced
                // if buttonDebounced[i]
                    // Perform very similar logic as around "increaseButtonCounts"
                    // and a few lines above, (see further up in the code) but instead for
                    // debouncedCounts 
                    ldi r18, HOLD_SAMPLE_LIMIT
                    cp r9, r18
                    brlo increaseDebouncedCount
                    rjmp nextIteration_updateButtonStates
                    increaseDebouncedCount:
                        // if debouncedCounts[i] < HOLD_SAMPLE_LIMIT
                        // Increase debouncedCounts[i] by 1
                        inc r9
                        st Z, r9
                        // If debouncedCounts[i] == HOLD_SAMPLE_LIMIT
                        cp r9, r18
                        brlo nextIteration_updateButtonStates
                        // Set buttonHeld[i] to 1
                        mov XH, r13
                        mov XL, r12
                        st X, r19
                        rjmp nextIteration_updateButtonStates
                // Else if !buttonDebounced[i]
                ifNotDebounced:
                    // For a button to be marked as tapped, it should fulfill:
                    // 1. debouncedCounts[i] >= 1
                    // 2. debouncedCounts[i] < TAP_SAMPLE_LIMIT
                    cp r9, r19
                    brlo resetDebouncedCounts
                    ldi r18, TAP_SAMPLE_LIMIT
                    cp r9, r18
                    brsh resetDebouncedCounts
                    // If we get here, (1) and (2) is true and we want to set
                    // buttonTapped[i] = 1
                    mov XH, r11
                    mov XL, r10
                    st X, r19
                    resetDebouncedCounts:
                        st Z, r1 // Set debouncedCounts[i] = 0
            // Update index counter and move on
            nextIteration_updateButtonStates:
                inc r17
                // Make buttonCounts[i] point to next item
                add r24, r19
                adc r25, r1
                // Do the same w/ debouncedCounts
                add r22, r19
                adc r23, r1
                // ...and w/ buttonDebounced
                add r20, r19
                adc r21, r1
                // ...and w/ buttonTapped
                add r10, r19
                adc r11, r1
                // ...and w/ buttonHeld
                add r12, r19
                adc r13, r1
                cp r17, r16
                brne for_updateButtonStates
        POP r8
        POP r9
        POP r10
        POP r11
        POP r12
        POP r13
        POP r14
        POP r16
        POP r17
        ret

    /* Get the current button that is pressed according to the ADC reading at an instant moment.
    This should be paired with the debouncing function updateButtonStates for proper button debouncing.
    Returns 0 if no button is pressed, else a number 1-5 corresponding to the pressed button.

    r25: Unused

    r24: The ADC reading that was sampled.

    Returns:
    r25: Empty

    r24: The corresponding button number that was detected.
    */
    getCurrentADCButton:
        ldi r23, 6 // Upper bound
        ldi r22, 0 // Index counter
        // Addresses of ADC_STATES
        ldi ZH, hi8(ADC_STATES)
        ldi ZL, lo8(ADC_STATES)
        // Iterate from i=0 through i=5
        for_getCurrentADCButton:
            // Load the ADC_STATES[i] value
            lpm r20, Z+
            // Check if <read ADC sample passed in r25-r24> >= ADC_STATES[i]
            // If yes, return
            cp r24, r20
            brsh return_getCurrentADCButton
            inc r22
            cp r22, r23
            brne for_getCurrentADCButton
        return_getCurrentADCButton:
            ldi r25, 0
            mov r24, r22
            ret

