/**
 *  Simple shake detection based on detected shocks.
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

#ifndef shake_detect_h
#define shake_detect_h

#include "accel.h"

#ifdef DEBUG
#include "softuart.h"
#include "debug_tools.h"
#endif

/*---------------------*/
/* SHAKE DETECT CONFIG */
/*---------------------*/

// threshold upon which beginShakeCallback is called when reached or endShakeCallback when it drops underneath again
#ifndef SHAKE_DETECT_THRESH
#define SHAKE_DETECT_THRESH 10
#endif

// maximum shake level. shakeLevel cannot rise higher than this value
#ifndef SHAKE_DETECT_MAX_LEVEL
#define SHAKE_DETECT_MAX_LEVEL	15
#endif

// threshold upon which a shock might be occured
#ifndef SHAKE_DETECT_SHOCK_THRESH
#define SHAKE_DETECT_SHOCK_THRESH 10000
#endif

#ifndef SHAKE_DETECT_SHOCK_LEVEL_INCREASE
#define SHAKE_DETECT_SHOCK_LEVEL_INCREASE 3
#endif

// minimum milli seconds of shake duration
// if you change this, you might need to change the timer prescaler mask below
#ifndef SHAKE_DETECT_MIN_MS
#ifdef DEBUG
#define SHAKE_DETECT_MIN_MS 125L	// in debug mode, the MC is "slower"
#else
#define SHAKE_DETECT_MIN_MS 500L
#endif
#endif

// ATtiny 8-bit timer prescaler mask
#ifndef SHAKE_DETECT_TIMER_PRESCALER_MASK
#define SHAKE_DETECT_TIMER_PRESCALER_MASK	((1 << CS11) | (1 << CS12) | (1 << CS13))	// set the timer prescaler to F_CPU / 8192
#define SHAKE_DETECT_TIMER_PRESCALER_VALUE	8192L
#endif

/*------------------------*/
/* SHAKE DETECT CALLBACKS */
/*------------------------*/

void (*shakeDetectBeginCallback)(void);  // shake begin callback
void (*shakeDetectEndCallback)(void);    // shake ended callback

/*------------------------*/
/* SHAKE DETECT FUNCTIONS */
/*------------------------*/

void shake_detect_init();

void shake_detect_update();

#endif