
/*
 * softwareConst.S
Includes software constants that is used throughout the project
 */ 
 .global TIMER_PRESCALER_VALUE, TICKS_PER_MS, MS_REQUIRED_FOR_CLICK, MS_REQUIRED_FOR_HOLD, TICKS_REQUIRED_FOR_CLICK, TICKS_REQUIRED_FOR_HOLD
 .data
	.align 2
	// ===== Timer constants =====
	// The clock selection to use for the timer, see bits CS[0:2] in TCR0B. Default is to use system clock but
	// with a prescaler of system clock / 64, which is equivalent to a the CS0:2 bits in TCR0B being set to 011.
	.equ TIMER_PRESCALER_VALUE, 0b011
	// Defines how many times the timer needs to tick up before 1 (one) ms has been reached.
	// This depends on your timer prescaler value and the clock frequency (see constants just above this)
	.equ TICKS_PER_MS, 124
	// ===== Button press detection constants =====
	// There are two different press types that can be detected:
	// (1) CLICKED - A "quick" clickety click on the button.
	// (2) HELD_DOWN - When the user holds down the button
	// Number of milliseconds that the system must have detected a certain button
	// via the ADC for it to be marked as clicked
	.equ MS_REQUIRED_FOR_CLICK, 50 
	// Number of milliseconds that the system must have detected a certain button
	// via the ADC for it to be marked as held down
	.equ MS_REQUIRED_FOR_HOLD, 1000
	// Since the timer counts ticks, convert the two variables above in terms of
	// number of ticks.
	.equ TICKS_REQUIRED_FOR_CLICK, MS_REQUIRED_FOR_CLICK*TICKS_PER_MS
	.equ TICKS_REQUIRED_FOR_HOLD, MS_REQUIRED_FOR_HOLD*TICKS_PER_MS