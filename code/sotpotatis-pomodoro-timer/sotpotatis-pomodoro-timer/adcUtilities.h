/* adcUtilities.h
Header declaring functions that can be found in the ASM file
adcUtilities.S
*/
#include <stdint.h>
#ifndef ADC_UTILITIES_H_
    #define ADC_UTILITIES_H_
        extern void setUpADC(uint8_t adcPinNumber, uint8_t modeToUse, uint8_t leftAdjustResults, uint8_t enableInterrupts, uint8_t prescalerSelectionValue);
		extern void clearADCInterrupts();
		extern uint8_t readCurrentADCValue();
#endif