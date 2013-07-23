/**
 *	Shaker firmware for Atmel AVR ATtiny .
 *	This program will recognize when it's shaken (i.e. the accelerometer is shaken)
 *	and will call the appropriate shakeBegan() / shakeEnded() functions.
 *	Configure the program by overwriting the "defines" in the header files.
 *
 *	Has been tested on ATtiny45 with 1Mhz and ADXL335 accelerometer.
 *
 *
 * Author: Markus Konrad <konrad@htw-berlin.de>
 * Organization: HTW Berlin, INKA Research Group, Project MINERVA
 * http://inka.htw-berlin.de/inka/projekte/minerva/
 * Copyright 2013. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification,are permitted provided that the following conditions
 * are met:
 *
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * Neither the name of the mkonrad.net nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <avr/wdt.h>
#include <util/delay.h>

#define WTD_TIME			WDTO_250MS
#define PIN_LED				PB0
#define PIN_AUDIO_TRIGGER	PB1

#define LED_SHAKEN_VAL		255
#define LED_SHAKEN_SPEED	5
#define LED_PULSE_MIN		0
#define LED_PULSE_MAX		20
#define LED_PULSE_SPEED		25

#ifdef DEBUG
#include "debug_tools.h"
#include "softuart.h"
// #define ADC_TEST_PIN	6
// #define ADC_TEST_CHAN	1
#endif

// #include "adc_tiny.h"
#include "accel.h"
#include "shake_detect.h"
#include "led_fader.h"

void shakeBegan(void) {
	led_fader_set(LED_FADER_MODE_FADE_TO, LED_SHAKEN_SPEED, LED_SHAKEN_VAL, 0);
	DIGIWRITE_H(PORTB, PIN_AUDIO_TRIGGER);
}

void shakeEnded(void) {
	led_fader_set(LED_FADER_MODE_ALTERNATING, LED_PULSE_SPEED, LED_PULSE_MIN, LED_PULSE_MAX);
	DIGIWRITE_L(PORTB, PIN_AUDIO_TRIGGER);
}

int main(void) {
	// set LED and audio trigger pins to output
	DDRB = (1 << PIN_LED)  | (1 << PIN_AUDIO_TRIGGER);
	DIGIWRITE_L(PORTB, PIN_AUDIO_TRIGGER);
	DIGIWRITE_L(PORTB, PIN_LED);

	// do initializations
	ACCEL_init();

#ifdef DEBUG
	initSoftUART();
#endif

	// calibrate accelerometer
	DIGIWRITE_H(PORTB, PIN_LED);
	ACCEL_calibrate();
	DIGIWRITE_L(PORTB, PIN_LED);

	// enable watchdog AFTER calibration
	wdt_enable(WTD_TIME);

	// set up shake detection
	shakeDetectBeginCallback = &shakeBegan;
	shakeDetectEndCallback = &shakeEnded;
	shake_detect_init();

	// set up LED fader
	led_fader_init();
	shakeEnded();	// will set the initial LED fading

	// main loop
	for ( ;; ) {
		shake_detect_update();	// sense for shaking
		wdt_reset();			// keep the watchdog happy
	}

	return 0; /* never reached */
}
