/*
hardwareConst.S
Contains hardware-related constants. This is the place to look if you want to change the pin mappings.
*/
#include <avr/io.h>
#define __SFR_OFFSET        0 // Need to redefine this for proper I/O address referencing
.global I2C_SDA_PIN, I2C_SCL_PIN, TIMER_INTERRUPT_PIN, LED_CHARLIEPLEXING_PIN_1, LED_CHARLIEPLEXING_PIN_2, LED_CHARLIEPLEXING_PIN_3, LED_CHARLIEPLEXING_PIN_4, INPUT_PINS, OUTPUT_PINS, PA_PIN_COUNT, PB_PIN_COUNT, PA_PIN_MASK, PB_PIN_MASK, BUTTON_ADC_PIN

.data
	.align 2
	// Define the different pins to use.
	.equ PA_PIN_COUNT, 8 // Number of PA pins
	.equ PB_PIN_COUNT, 4 // Number of PB pins
	.equ PA_PIN_MASK, 0xFF >> (8-PA_PIN_COUNT)
	.equ PB_PIN_MASK, 0xFF >> (8-PB_PIN_COUNT)
	// For Pure I/O pins (not ADC-pins), each "_PIN" definition follows the following format:
	// The Attiny24A has 12 GPIO pins. Starting from PA we define a 12-bit number:
	// 0bPA7|PA6....PA0|PB3|...PB0
	// Where one and only one bit is high to control one pin.
	// For instance, 0b0000 0001 0000 maps a label to PA0.
	// It points to a 2x integer list where the first element reprensents:
	// 0: The pin is connected to PORTA, 1: The pin is connected to PORTB
	// And the next entry in the list is the actual pin that is being used.
	// ====== LED pins ======
	// Note that we use LED Charlieplexing, so that one
	// pin controls 3 LEDs
	.equ LED_CHARLIEPLEXING_PIN_1, (1<<PA0<<8)
	.equ LED_CHARLIEPLEXING_PIN_2, (1<<PA1<<8)
	.equ LED_CHARLIEPLEXING_PIN_3, (1<<PA2<<8)
	.equ LED_CHARLIEPLEXING_PIN_4, (1<<PA3<<8)
	// ====== I2C pins ======
	.equ I2C_SDA_PIN, (1<<PA6<<8)
	.equ I2C_SCL_PIN, (1<<PA4<<8)
	// ====== Timer interrupt pin ======
	.equ TIMER_INTERRUPT_PIN, (1<<PB3)
	// ====== Button ADC pin ======
	// Note that ADC pins simply use the format defined by Microchip
	// (0-7 depending on if you want to use pin PA0-7)
	.equ BUTTON_ADC_PIN, PB3
	// ====== Helper constants ======
	// All input and output pins
	.equ INPUT_PINS, 0
	.equ OUTPUT_PINS, LED_CHARLIEPLEXING_PIN_1 | LED_CHARLIEPLEXING_PIN_2 | LED_CHARLIEPLEXING_PIN_3 | LED_CHARLIEPLEXING_PIN_4