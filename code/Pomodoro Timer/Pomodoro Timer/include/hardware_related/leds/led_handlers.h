/* led_handlers.h
Header declaring functions that can be found in the ASM file
led_handlers.S.
*/
#ifndef LED_HANDLERS_H_
    #define LED_HANDLERS_H_
		extern "C" {
			void set_charlieplexing_state(uint8_t led_number);
			void reset_all_charlieplexing_pins();
		}
#endif
