# ADC multiplexing tests

The files in this directory are as follows:

1. `adc_muliplexing_schematics.fzz` - A Fritzing project which has the schematics of the ADC multiplexing circuits and a stripboard translation of it.
However, it has never been built, so it might be incorrect.

> **ℹ️Note:** The main algorithm for ADC multiplexing was taught to me by patient AVRFreaks forums user `avrcandies`. I thank you very very much for the help!
> [This is the topic that they helped me in](www.avrfreaks.net/s/topic/a5CV40000003RbZMAU/t400824?page=2).

2. `Arduino Tests` - Contains test sketches used on an Arduino Uno to validate the multiplexing circuit and the actual algorithm.

3. `C Code, migrated to ASM nowadays` - Contains source code of `getCurrentADCButton` and `updateButtonStates` functions before they were translated to Assembly. 
Their Assembly translations have made it into the "real" source code, see adcMultiplexing.S.
