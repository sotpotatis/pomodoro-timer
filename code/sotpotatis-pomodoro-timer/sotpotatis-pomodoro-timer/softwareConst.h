/*
 * softwareConst.h
 * Includes software-related constants used throughout the project.
 */ 

#include <stdint.h>
#ifndef SOFTWARECONST_H_
	#define SOFTWARECONST_H_
		// ===== Timer constants =====
		// The clock selection to use for the timer, see bits CS[0:2] in TCR0B. Default is to use system clock but
		// with a prescaler of system clock / 64, which is equivalent to a the CS0:2 bits in TCR0B being set to 011.
		#define TIMER_PRESCALER_VALUE 0b011
		// Defines how many times the timer needs to tick up before 1 (one) ms has been reached.
		// This depends on your timer prescaler value and the clock frequency (see constants just above this)
		#define TICKS_PER_MS 124
		// ===== ADC Button press detection constants =====
		// Button press detection is done via ADC, by sampling the ADC every n ms.
		#define ADC_SAMPLE_RATE 5 // The millisecond delay between each ADC sample. Must be >= 1ms due to how the code currently works
		#define ADC_PRESCALER_VALUE 7 // The prescaler to use for the ADC. Defaults to 7 (1/128 prescaler
		// There are two different press types that can be detected:
		// (1) TAPPED - A "quick" clickety click on the button.
		// (2) HELD - When the user holds down the button
		// The thresholds for these detections can be found below
		// Button n is declared debounced after the ADC matched the buttons voltage level
		// for DEBOUNCE_SAMPLE_LIMIT ADC samples.
		#define DEBOUNCE_SAMPLE_LIMIT 10
		// A button is declared held if it has been declared debounced = true for HOLD_SAMPLE_LIMIT number of
		// ADC samples.
		#define HOLD_SAMPLE_LIMIT 150
		// A button is declared tapped if it has been declared debounced = true for **maximum** TAP_SAMPLE_LIMIT number of
		// ADC samples.
		#define TAP_SAMPLE_LIMIT 80
#endif