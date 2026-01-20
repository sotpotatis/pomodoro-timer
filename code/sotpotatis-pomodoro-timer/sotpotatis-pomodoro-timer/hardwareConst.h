/* hardwareConst.h
Defines hardware-related constants.
*/
#include <avr/io.h>
#ifndef HARDWARECONST_H_
	#define HARDWARECONST_H_
		// This is part of <avr/io.h> and we need to redefine this for proper I/O address referencing
		#undef __SFR_OFFSET
		#define __SFR_OFFSET  0
		// ====== ADC Pin to use for button multiplexing ======
		// Note that ADC pins simply use the format defined by Microchip
		// (0-7 depending on if you want to use pin PA0-7)
		#define BUTTON_ADC_PIN 5
		// For Pure I/O pins (not ADC-pins), each "_PIN" definition follows the following format:
		// The Attiny24A has 12 GPIO pins. Starting from PA we define a 16 bit number:
		// 0bPA7|PA6....PA0|constant 0000 PB3|...PB0 (for example: 0b0000 0011 0000 0000 refers to PA0 and PA1)
		// Where one and only one bit is high to control one pin.
		// For instance, 0b0000 0001 0000 maps a label to PA0.
		// It points to a 2x integer list where the first element represents:
		// 0: The pin is connected to PORTA, 1: The pin is connected to PORTB
		// And the next entry in the list is the actual pin that is being used.
		// ====== LED pins ======
		// Note that we use LED Charlieplexing, so that one
		// pin controls 3 LEDs. The format of pin numbers is according to above,
		// that is why we left shift by 8.
		#define LED_CHARLIEPLEXING_PIN_1 0x0001 // PB0
		#define LED_CHARLIEPLEXING_PIN_2 0x0002 // PB1
		#define LED_CHARLIEPLEXING_PIN_3 0x8000 // PA7
		#define LED_CHARLIEPLEXING_PIN_4 0x4000 // PA6
		#define LED_CHARLIEPLEXING_PIN_5 0x0100// PA0
		// Operates only on 8 bit registers
		#define PA_PIN_MASK 0xFF
		#define PB_PIN_MASK 0x0F
#endif 