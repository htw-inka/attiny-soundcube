#include "standbytimer.h"

#include "adc_tiny.h"
#include "shake_detect.h"

static uint32_t ovfCount;		// overflow counter
static uint32_t ovfCounterMax;	// maximum overflow counter value
static uint32_t voltSum;		// voltage sum measured on CHAN_ANALOG_SIG

void standby_timer_init() {
	// set timer register
	uint32_t f1 = (uint32_t)((int64_t)F_CPU / SHAKE_DETECT_TIMER_PRESCALER_VALUE);	// = 122
	uint32_t f2 = (uint32_t)((STANDBY_TIMER_SEC * 100) / 256);						// = 117
	
	ovfCounterMax = (uint32_t)((f1 * f2) / (uint32_t)100);							// = 100
	
//	ovfCounterMax = (uint16_t)((int64_t)STANDBY_TIMER_SEC * (int64_t)F_CPU)
//					/ ((int64_t)256 * (int64_t)SHAKE_DETECT_TIMER_PRESCALER_VALUE);	// we share the timer with shake_detect!
	
	// set interrupt register
	STANDBY_INTERRUPT_REG |= STANDBY_INTERRUPT_REG_MASK;
	
	// enable interrupts
	sei();
}

void standby_timer_reset() {
	ovfCount = 0;
	voltSum = 0;
}

ISR(STANDBY_INTERRUPT_HANDLER) {	
	ovfCount++;
	
	// add measurement to voltage sum
	uint32_t v = adc_read_avg(STANDBY_VOLT_READ_CHAN, 5);
	voltSum += v;
	
	if (ovfCount == ovfCounterMax) {	// we waited long enough! (STANDBY_TIMER_SEC seconds)
		// calculate average voltage and call the callback function	
		(*standbyTimerCallback)(voltSum / ovfCounterMax);
	} else {
		(*stationCheckCallback)(v);	// not yet...
	}
}