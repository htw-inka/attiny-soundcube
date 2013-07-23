/**
 *	PWM based LED fader functions.
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

#ifndef LED_FADER_H
#define LED_FADER_H

#include <avr/io.h>
#include <avr/interrupt.h>

/*------------------*/
/* LED FADER CONFIG */
/*------------------*/

#ifndef LED_FADER_8BIT_TIMER_REGA
#define LED_FADER_8BIT_TIMER_REGA 		TCCR0A
#define LED_FADER_8BIT_TIMER_REGA_MASK	(1<<COM0A1) | (1<<WGM00)	// Use OC0A (Pin PB0), PWM, Phase Correct, OCRA0 at TOP
#endif
#ifndef LED_FADER_8BIT_TIMER_REGB
#define LED_FADER_8BIT_TIMER_REGB 		TCCR0B
#define LED_FADER_8BIT_TIMER_REGB_MASK 	(1<<CS01)					// Prescaler 8
#endif

#ifndef LED_FADER_INTERRUPT_REG
#define LED_FADER_INTERRUPT_REG 		TIMSK
#define LED_FADER_INTERRUPT_REG_MASK 	(1<<OCIE0A)					// Allow compare interrupt
#endif

#ifndef LED_FADER_COMP_REG
#define LED_FADER_COMP_REG		OCR0A
#endif

#ifndef LED_FADER_COMP_TIMER_IR
#define LED_FADER_COMP_TIMER_IR TIMER0_COMPA_vect
#endif

/*-------------*/
/* FADER MODES */
/*-------------*/
#define LED_FADER_MODE_FADE_TO		0
#define LED_FADER_MODE_ALTERNATING	1

/*-----------------*/
/* FADER FUNCTIONS */
/*-----------------*/

/**
 *	Initialize LED fader. The LED pin must be already set to "output"
 *	in the DDR!
 */
void led_fader_init();

/**
 *	Set a LED fader mode <mode> and its values <v1> and <v2>. The fade will
 *	fade with <speed> where 1 is the fastest and 255 is the slowest speed.
 *	<mode> must be one of LED_FADER_MODE_*.
 */
void led_fader_set(uint8_t mode, uint8_t speed, uint8_t v1, uint8_t v2);

#endif