#include "tools_tiny.h"

void long_delay_ms(uint16_t ms)
{
    for(; ms>0; ms--) _delay_ms(1);
}

void blinkLED(volatile uint8_t * const prt, uint8_t pin, uint16_t ms) {
	DIGIWRITE_H(*prt, pin);
	long_delay_ms(ms);
	DIGIWRITE_L(*prt, pin);
}