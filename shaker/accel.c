#include "ACCEL.h"

#ifdef DEBUG
#include "debug_tools.h"
#include "softuart.h"
#endif

static uint16_t ACCEL_offSets[3] = {0, 0, 0};

#ifdef ACCEL_PIN_GSELECT
static uint8_t ACCEL_sensi;
#endif

void ACCEL_init() {
#ifdef ACCEL_CALL_ADC_INIT
    adc_init();
#endif

	// set outputs
#ifdef ACCEL_PIN_SLEEP
	ACCEL_DDR |= (1 << ACCEL_PIN_SLEEP);
#endif
#ifdef ACCEL_PIN_GSELECT
	ACCEL_DDR |= (1 << ACCEL_PIN_GSELECT);
#endif

	// inputs
	ACCEL_DDR &= ~( (1 << ACCEL_PIN_X)
             	  | (1 << ACCEL_PIN_Y)
             	  | (1 << ACCEL_PIN_Z) );

    // set initial states
#ifdef ACCEL_PIN_SLEEP
    ACCEL_PORT_OUT |= (1 << ACCEL_PIN_SLEEP);
#endif
}

#ifdef ACCEL_CHAN_X
uint16_t ACCEL_getXRaw() {
	return adc_read_avg(ACCEL_CHAN_X, ACCEL_AVG_SAMPLES);
}
#endif

uint16_t ACCEL_getYRaw() {
    return adc_read_avg(ACCEL_CHAN_Y, ACCEL_AVG_SAMPLES);
}

uint16_t ACCEL_getZRaw() {
    return adc_read_avg(ACCEL_CHAN_Z, ACCEL_AVG_SAMPLES);
}

#ifdef ACCEL_CHAN_X
int16_t ACCEL_getXAccel() {
    return (int16_t)ACCEL_offSets[0] - (int16_t)ACCEL_getXRaw();
}
#endif

int16_t ACCEL_getYAccel() {
    return (int16_t)ACCEL_offSets[1] - (int16_t)ACCEL_getYRaw();
}

int16_t ACCEL_getZAccel() {
    return (int16_t)ACCEL_offSets[2] - (int16_t)ACCEL_getZRaw();
}

int32_t ACCEL_getTotalVectorSquared() {
    // sometimes, this seems to cause a integer-overflow:
#ifdef ACCEL_CHAN_X
	return SQUARE((int32_t)ACCEL_getXAccel())
		 + SQUARE((int32_t)ACCEL_getYAccel())
		 + SQUARE((int32_t)ACCEL_getZAccel());
#else
	return SQUARE((int32_t)ACCEL_getYAccel())
		 + SQUARE((int32_t)ACCEL_getZAccel());
#endif
}

void ACCEL_calibrate() {
#ifdef ACCEL_CHAN_X
    uint32_t sumX = 0;
#endif
    uint32_t sumY = 0;
    uint32_t sumZ = 0;
    uint16_t i = 0;

    for (; i < ACCEL_CALIB_SAMPLES; i++) {
#ifdef ACCEL_CHAN_X    
        sumX += ACCEL_getXRaw();
#endif
        sumY += ACCEL_getYRaw();
        sumZ += ACCEL_getZRaw();
#ifdef DEBUG
        if (i % 100 == 0) softuart_putchar('.');
#endif
    }
/*#ifdef DEBUG
    PRINT_NL;
    printInt((uint16_t) (sumX / ACCEL_CALIB_SAMPLES)); PRINT_NL;
    printInt((uint16_t) (sumY / ACCEL_CALIB_SAMPLES)); PRINT_NL;
    printInt((uint16_t) (sumZ / ACCEL_CALIB_SAMPLES)); PRINT_NL;
#endif*/

#ifdef ACCEL_CHAN_X    
    ACCEL_offSets[0] = (uint16_t) (sumX / ACCEL_CALIB_SAMPLES);
#endif
    ACCEL_offSets[1] = (uint16_t) (sumY / ACCEL_CALIB_SAMPLES);
    ACCEL_offSets[2] = (uint16_t) (sumZ / ACCEL_CALIB_SAMPLES);
}

#ifdef ACCEL_PIN_GSELECT
void ACCEL_setSensitivity(uint8_t sensi) {
    ACCEL_sensi = sensi;
    DIGIWRITE(ACCEL_PORT_OUT, ACCEL_PIN_GSELECT, !sensi);
}
#endif

#ifdef ACCEL_PIN_SLEEP
void ACCEL_sleep() {
    DIGIWRITE(ACCEL_PORT_OUT, ACCEL_PIN_GSELECT, 0);
}

void ACCEL_wake() {
    DIGIWRITE(ACCEL_PORT_OUT, ACCEL_PIN_GSELECT, 1);
    _delay_ms(2);
}
#endif