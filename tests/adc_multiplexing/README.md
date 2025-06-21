# ADC multiplexing tests

The files in this directory are as follows:

1. `adc_muliplexing_schematics.fzz` - A Fritzing project which has the schematics of the ADC multiplexing circuits and a stripboard translation of it.
However, it has never been built, so it might be incorrect.

2. `Arduino Tests` - Contains test sketches used on an Arduino Uno to validate the multiplexing circuit and the actual algorithm.

3. `C Code, migrated to ASM nowadays` - Contains source code of `getCurrentADCButton` and `updateButtonStates` functions before they were translated to Assembly. 
Their Assembly translations have made it into the "real" source code, see adcMultiplexing.S.
