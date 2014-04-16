/**
 *	Shaker firmware for Atmel AVR ATtiny .
 *	This program will recognize when it's shaken (i.e. the accelerometer is shaken)
 *	and will call the appropriate shakeBegan() / shakeEnded() functions.
 *	Configure the program by overwriting the "defines" in the header files.
 *
 *	Has been tested on ATtiny45/85 with 1Mhz and ADXL335 accelerometer.
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

//#include <avr/wdt.h>	// TODO: reenable
#include <avr/sleep.h>
#include <util/delay.h>

#define WTD_TIME			WDTO_250MS
#define PIN_LED				PB0
#define PIN_AUDIO_TRIGGER	PB1

#define LED_SHAKEN_VAL		255
#define LED_SHAKEN_SPEED	10
#define LED_PULSE_MIN		10
#define LED_PULSE_MAX		175
#define LED_PULSE_SPEED		5

#ifdef DEBUG
#include "debug_tools.h"
#include "softuart.h"
// #define ADC_TEST_PIN	6
// #define ADC_TEST_CHAN	1
#endif

#include "accel.h"
#include "shake_detect.h"
#include "led_fader.h"

#ifdef ENABLE_SLEEP
#include "standbytimer.h"

static uint8_t fullLight = 0;
#endif

void startLEDDefaultMode(void) {
	// Pulsating LED
	led_fader_enable();
	led_fader_set(LED_FADER_MODE_ALTERNATING, LED_PULSE_SPEED, LED_PULSE_MIN, LED_PULSE_MAX);
}

void shakeBegan(void) {
#ifdef ENABLE_SLEEP
	standby_timer_reset();
#endif
	
	// fade up
	led_fader_set(LED_FADER_MODE_FADE_TO, LED_SHAKEN_SPEED, LED_SHAKEN_VAL, 0);
	DIGIWRITE_H(PORTB, PIN_AUDIO_TRIGGER);	// give signal to start audio playback
    
#ifdef ENABLE_SLEEP
	fullLight = 1;
#endif
}

void shakeEnded(void) {
	DIGIWRITE_L(PORTB, PIN_AUDIO_TRIGGER);	// stop giving signal to audio playback

#ifndef ENABLE_SLEEP
	led_fader_set(LED_FADER_MODE_ALTERNATING, LED_PULSE_SPEED, LED_PULSE_MIN, LED_PULSE_MAX);
#endif
}

#ifdef ENABLE_SLEEP
void onStationCheck(uint32_t avgInputVolt) {
	if (avgInputVolt < 256 && fullLight == 1) {	// we're on the station again, bat. is loading!
    	standby_timer_reset();

		// fade down again
		led_fader_set(LED_FADER_MODE_ALTERNATING, LED_PULSE_SPEED, LED_PULSE_MIN, LED_PULSE_MAX);
		
		fullLight = 0;
        
        DIGIWRITE_L(PORTB, PIN_AUDIO_TRIGGER);  // stop giving signal to audio playback
	}
}

void standbyTimerDone(uint32_t avgInputVolt) {	
	if (avgInputVolt > 768) { // high level means NO bat. loading!  now we can really go to sleep
		// shut down everything
		led_fader_disable();
		DIGIWRITE_L(PORTB, PIN_LED);
        DIGIWRITE_L(PORTB, PIN_AUDIO_TRIGGER);  // stop giving signal to audio playback
		DIGIWRITE_L(PORTB, ACCEL_PIN_X);
		
		// prepare going to sleep
		GIMSK |= (1 << INT0);	// enable external interrupt on PB2
		
		// NOTE: only LOW level will wake the MCU up again!
		// this does not work somehow:
//		MCUCR |= (1 << ISC01) | (1 << ISC00);	// The rising edge of INT0 generates an interrupt request.
//		MCUCR |= (1 << ISC00);	// Any logical change on INT0 generates an interrupt request.
//		wdt_disable();
				
		// go to sleep
		sleep_enable();
		sei();
		sleep_bod_disable();
        sleep_cpu();
        
		// woken up!
		cli();
        sleep_disable();
		
		GIMSK &= ~(1 << INT0);	// disable external interrupt on PB2
		
		// test LEDs. 2 times blinking means "woken up"
		blinkLED(&PORTB, PIN_LED, 50);
		long_delay_ms(50);
		blinkLED(&PORTB, PIN_LED, 50);

		// start LED fading again
		startLEDDefaultMode();
		shakeEnded();		// reset states
		
		// wake up watch dog
//		wdt_enable(WTD_TIME);
	} else {
		standby_timer_reset();		// try it again
	}
	
	// standby_timer_enable();
}
#endif  // #ifdef ENABLE_SLEEP

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
//	wdt_enable(WTD_TIME);

	// set up shake detection
	shakeDetectBeginCallback = &shakeBegan;
	shakeDetectEndCallback = &shakeEnded;
	shake_detect_init();

#ifdef ENABLE_SLEEP
	// set up standby timer
	standby_timer_init();
	stationCheckCallback = &onStationCheck;
	standbyTimerCallback = &standbyTimerDone;
	
	// set default sleep mode
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
#endif // #ifdef ENABLE_SLEEP

	// set up LED fader
	led_fader_init();
	startLEDDefaultMode();
	shakeEnded();		// reset states

	DIGIWRITE_H(PORTB, PIN_LED);

	// main loop
	for ( ;; ) {
		shake_detect_update();	// sense for shaking
		// wdt_reset();			// keep the watchdog happy
	}

	return 0; /* never reached */
}

ISR(INT0_vect) {
	// called when woken up (PB2 changed to LOW level)
	// but not used here!!
}