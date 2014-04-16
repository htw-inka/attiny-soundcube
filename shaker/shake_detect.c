#include "shake_detect.h"

#include "tools_tiny.h"
#include "standbytimer.h"

/*--------------------------------*/
/* SHAKE DETECT PRIVATE VARIABLES */
/*--------------------------------*/

static uint8_t shakeLevel = 0;				// current shake level
static uint8_t shakeDetected = 0;			// is 0 if no shake was detected
static uint8_t minShakeTimeExceeded = 0;	// is 1 if minimum shake time is exceeded
static uint8_t minShakeTimeCounterVal;		// counter value for SHAKE_DETECT_MIN_MS

/*---------------------------------------*/
/* SHAKE DETECT FUNCTION IMPLEMENTATIONS */
/*---------------------------------------*/

void shake_detect_timer_enable() {
	
}

void shake_detect_timer_disable() {
	
}

void shake_detect_init() {
	// this is in theory: 1000000 * 1000 / 8192 / 500 - 1
	minShakeTimeCounterVal = (F_CPU * 1000L) / SHAKE_DETECT_TIMER_PRESCALER_VALUE / SHAKE_DETECT_MIN_MS - 1;

	// set up the timer
	SHAKE_DETECT_TIMER_REG = SHAKE_DETECT_TIMER_PRESCALER_MASK;
}

void shake_detect_update() {
	// if (shakeDetected == 0) SHAKE_DETECT_TIMER_COUNTER = 0;	// reset timer when no shake was detected

	const int32_t totalG = ACCEL_getTotalVectorSquared();

	if (shakeDetected == 1 && minShakeTimeExceeded == 0
	 && SHAKE_DETECT_TIMER_COUNTER >= minShakeTimeCounterVal)  // it was shaken long enough
	{	
		minShakeTimeExceeded = 1;
		// DIGIWRITE_H(PORTB, PB1);
	}

	if (shakeLevel > 0) {	// when a shake level is given, 
		shakeLevel--;	// calm down again
#ifdef DEBUG
		softuart_putchar('l');
		softuart_putchar('=');
		printInt(shakeLevel);PRINT_NL;
#endif
	}

	// check if we can end the shake event
	if (shakeLevel < SHAKE_DETECT_THRESH && shakeDetected != 0 && minShakeTimeExceeded != 0) {
#ifdef DEBUG
		softuart_putchar('s');PRINT_NL;
#endif
		shakeDetected = 0;
		// DIGIWRITE_L(PORTB, PB1);
		minShakeTimeExceeded = 0;
		(*shakeDetectEndCallback)();
	} else if (totalG > SHAKE_DETECT_SHOCK_THRESH
	        && shakeLevel + SHAKE_DETECT_SHOCK_LEVEL_INCREASE < SHAKE_DETECT_MAX_LEVEL)
	{
        shakeLevel += SHAKE_DETECT_SHOCK_LEVEL_INCREASE;	// get more excited

#ifdef DEBUG
		// printUInt(totalG);PRINT_NL;
		softuart_putchar('l');
		softuart_putchar('=');
		printInt(shakeLevel);PRINT_NL;
#endif

		// check if we can start the shake event
		if (shakeLevel >= SHAKE_DETECT_THRESH && shakeDetected == 0) {
#ifdef DEBUG
			softuart_putchar('S');PRINT_NL;
#endif
			shakeDetected = 1;
			SHAKE_DETECT_TIMER_COUNTER = 0;	// reset the timer
			(*shakeDetectBeginCallback)();
		}
	}
}