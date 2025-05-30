#ifndef ADC_UTILITIES_H_
    #define ADC_UTILITIES_H_
        extern void setUpADC(unsigned char adcPinNumber, unsigned char modeToUse, unsigned char leftAdjustResults, unsigned char enableInterrupts, unsigned char prescalerSelectionValue);
		extern void clearADCInterrupts();
#endif