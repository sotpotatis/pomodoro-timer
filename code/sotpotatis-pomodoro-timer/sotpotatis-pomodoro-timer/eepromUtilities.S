/* eepromUtilities.S
Utilities for reading and writing to the EEPROM memory.
*/
#include "hardwareConst.h"
.global writeEEPROM, readEEPROM
.text
/*
Utility function that waits until the EEPROM is ready,
and then copies the address given by the input argument to the
EEARX registers (which store the EEPROM addresses).
Since this is an operation that is done both by the
EEPROM write and read operations, it is a separate function to avoid
code duplication.
r25: The high part of the EEPROM address. 1 bit value. Either 1 or 0.
r24: The lower part of the EEPROM address.
*/
waitUntilEEPROMReadyAndCopyAddress:
	// Mask inputs
	andi r25, 1
	// Ensure EEPROM is ready
	untilReady:
		// This is a cool one.
		// It skips the next instruction (i.e.
		// the jump back to untilReady) if the bit
		// EEPE (bit 1) is cleared. EEPE is nonzero
		// if a program operation is pending
		sbic EECR, 1
		rjmp untilReady
	// Copy the address the user wants to write to.
	// Copy into the EEARX registers.
	out EEARH, r25
	out EEARL, r24
	ret

/* Writes data to the EEPROM memory.
NOTE: This function internally disables global interrupts, and then
re-enables them before return.
Takes the following arguments:
r25: The high part of the EEPROM address. 1 bit value. Either 1 or 0.
r24: The lower part of the EEPROM address.
r23: Unused
r22: The data to write to the EEPROM.
*/
writeEEPROM:
	cli // Disable interrupts
	rcall waitUntilEEPROMReadyAndCopyAddress
	// Copy the data that the user wants to write to EEDR.
	out EEDR, r22
	// Now, do the following in order to initiate the write:
	// 1. Set the programming mode - to Erase & Write in one op.
	// Done by settting EEPM1 and EEPM0 (bit 5-4 in EECR) to 0.
	// 2. Set the EEMPE bit (bit 2 in EECR) to 1
	// 3. Set the EEPE bit in (bit 1 in EECR) to 1
	// (2 and 3 start the EEPROM write operation)
	in r25, EECR
	// Clear bit 5 and 4 as described above. Regarding bit 6 and 7,
	// the Attiny84A datashet says that bit 6 will always read as 0
	// and bit 7 should be masked out. Therefore we mask out
	// the whole upper nibble of EECR.
	andi r25, 0x0F
	ori r25, 4 // Set bit 2, the EEMPE bit, to 1.
	out EECR, r25
	sbi EECR, 1 // Set the EEPE bit to 1
	sei // Re-enable interupts
	ret

/* Reads data from the EEPROM memory.
Takes the following arguments:
r25: The high part of the EEPROM address. 1 bit value. Either 1 or 0.
r24: The lower part of the EEPROM address.
Returns the read data in r25 and r24. r25 = 0, r24 = 8 bits of read data.
*/
readEEPROM:
	rcall waitUntilEEPROMReadyAndCopyAddress
	// Write the EERE bit (bit 0 in EECR) to 1 to start the read.
	sbi EECR, 0
	// Load the read data from r24. (it should now be available in the EEDR register)
	in r24, EEDR
	ldi r25, 0 // To follow ASM-C-calling conventions, ensure r25 (upper 8 bits
	// of return argument is 0)
	ret

	