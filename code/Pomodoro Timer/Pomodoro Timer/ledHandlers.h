/* ledHandlers.h
Header declaring functions that can be found in the ASM file
ledHandlers.S.
*/
#ifndef LEDHANDLERS_H_
    #define LEDHANDLERS_H_
		extern "C" {
			void setCharlieplexingState(uint8_t ledNumber);
			void resetAllCharlieplexingPins();
		}
#endif