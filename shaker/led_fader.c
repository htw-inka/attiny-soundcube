#include "led_fader.h"

static volatile uint8_t fadeIdleCnt = 0;
static volatile uint8_t compVal;
static volatile int8_t compValChange = 1;
static uint8_t compValMax = 255;
static uint8_t compValMin = 0;
static uint8_t fadeMode = LED_FADER_MODE_FADE_TO;
static uint8_t fadeSpeed = 1;

void led_fader_init() {
	// set timer registers
	LED_FADER_8BIT_TIMER_REGA = LED_FADER_8BIT_TIMER_REGA_MASK;
	LED_FADER_8BIT_TIMER_REGB = LED_FADER_8BIT_TIMER_REGB_MASK;

	// set interrupt register
	LED_FADER_INTERRUPT_REG = LED_FADER_INTERRUPT_REG_MASK;

	// set initial values
	compVal = compValMin;
	LED_FADER_COMP_REG = compVal;

	// enable interrupts
	sei();
}

void led_fader_set(uint8_t mode, uint8_t speed, uint8_t v1, uint8_t v2) {
	fadeMode = mode;
	fadeSpeed = speed;
	compValMin = v1;
	compValMax = v2;

	if (fadeMode == LED_FADER_MODE_FADE_TO) {
		if (compVal > compValMin) compValChange = -1;
		else if (compVal == compValMin) compValChange = 0;
		else compValChange = 1;
	} else if (fadeMode == LED_FADER_MODE_ALTERNATING) {
		if (compVal > compValMax) compValChange = -1;
		else compValChange = 1;
	}
}

ISR(LED_FADER_COMP_TIMER_IR) {
	fadeIdleCnt++;
	
	if (fadeIdleCnt == fadeSpeed) {
		compVal += compValChange;

		if (fadeMode == LED_FADER_MODE_FADE_TO && compVal == compValMin) {
			compValChange = 0;
		} else if (fadeMode == LED_FADER_MODE_ALTERNATING) {		
			if (compVal == compValMax) {
				compValChange = -1;
			} else if (compVal == compValMin) {
				compValChange = 1;
			}
		}
		
		fadeIdleCnt = 0;
	}
	
	LED_FADER_COMP_REG = compVal;
}