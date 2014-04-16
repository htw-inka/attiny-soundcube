/**
 *	General lo-fi accelerometer driver for ATtiny
 *	Has been tested with ADXL335 accelerometer.
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

#ifndef ACCEL_H
#define ACCEL_H

#include <avr/io.h>
#include <util/delay.h>

#include "tools_tiny.h"
#include "adc_tiny.h"

#define ACCEL_AREF_V_5		0
#define ACCEL_AREF_V_3_3	1

/*-----------------------*/
/* ACCEL DRIVER CONFIG */
/*-----------------------*/

#define ACCEL_CALL_ADC_INIT		// define/undefine if on ACCEL_init() adc_init() should be called or not

#ifndef ACCEL_AREF_V
#define ACCEL_AREF_V ACCEL_AREF_V_3_3
#endif

#ifndef ACCEL_INITIAL_SENSITIVITY		// sensi = 0: low sensitivity
#define ACCEL_INITIAL_SENSITIVITY 0		// sensi = 1: high sensitivity
#endif

#ifndef ACCEL_AVG_SAMPLES
#define ACCEL_AVG_SAMPLES 10
#endif

#ifndef ACCEL_CALIB_SAMPLES
#define ACCEL_CALIB_SAMPLES 250
#endif

// #ifndef ACCEL_PIN_SLEEP
// #define ACCEL_PIN_SLEEP 0
// #endif

// #ifndef ACCEL_PIN_GSELECT
// #define ACCEL_PIN_GSELECT 0
// #endif

#ifndef ACCEL_DDR
#define ACCEL_DDR	DDRB
#endif

#ifndef ACCEL_PIN_IN
#define ACCEL_PIN_IN PINB
#endif

#ifndef ACCEL_PORT_OUT
#define ACCEL_PORT_OUT PORTB
#endif

// Pin X is not used for shake detection! (See below.)

#ifndef ACCEL_PIN_X
#define ACCEL_PIN_X PB2
#endif

#ifndef ACCEL_PIN_Y
#define ACCEL_PIN_Y PB4
#endif

#ifndef ACCEL_PIN_Z
#define ACCEL_PIN_Z PB3
#endif

// Pin X (Channel 1) is not used for shake detection!
// We need it for battery loading detection.
// Uncomment to use it again.

//#ifndef ACCEL_CHAN_X
//#define ACCEL_CHAN_X 1
//#endif
#undef ACCEL_CHAN_X

#ifndef ACCEL_CHAN_Y
#define ACCEL_CHAN_Y 2
#endif

#ifndef ACCEL_CHAN_Z
#define ACCEL_CHAN_Z 3
#endif

/*-----------*/
/* FUNCTIONS */
/*-----------*/

/**
 * Initialize driver with above options
 */
void ACCEL_init();

#ifdef ACCEL_CHAN_X
uint16_t ACCEL_getXRaw();
#endif
uint16_t ACCEL_getYRaw();
uint16_t ACCEL_getZRaw();

#ifdef ACCEL_CHAN_X
int16_t ACCEL_getXAccel();
#endif
int16_t ACCEL_getYAccel();
int16_t ACCEL_getZAccel();

int32_t ACCEL_getTotalVectorSquared();

void ACCEL_calibrate();

#ifdef ACCEL_PIN_GSELECT
void ACCEL_setSensitivity(uint8_t sensi);
#endif

#ifdef ACCEL_PIN_SLEEP
void ACCEL_sleep();
void ACCEL_wake();
#endif

#endif