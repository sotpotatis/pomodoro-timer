#ifndef ADC_UTILITIES_H_
    #define ADC_UTILITIES_H_
        extern void setUpADC(unsigned char adcPinNumber, unsigned char triggerSource, unsigned char presentResultLeftAdjusted, unsigned char enableADCInterrupts, unsigned char adcPrescalerValue);
		extern void clearADCInterrupts();
        extern const unsigned char ADC_TRIGGER_SOURCE_FREE_RUNNING_MODE;
		extern void waitForInterrupts();
#endif