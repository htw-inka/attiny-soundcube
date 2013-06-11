/**
 *	Common tools for ATtiny.
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

#ifndef tools_tiny_h
#define tools_tiny_h

#include <avr/io.h>
#include <util/delay.h>

#ifndef DIGIWRITE_H
#define DIGIWRITE_H(prt, pn) prt |= (1<<pn)
#endif

#ifndef DIGIWRITE_L
#define DIGIWRITE_L(prt, pn) prt &= ~(1<<pn)
#endif

#ifndef DIGIWRITE
#define DIGIWRITE(prt, pn, v) if ( v==0 ) prt &= ~(1<<pn); else prt |= (1<<pn)
#endif

#ifndef SQUARE
#define SQUARE(x) ((x) * (x))
#endif

/**
 *	Implements a long delay function for wating longer than ~10ms
 */
inline void long_delay_ms(uint16_t ms);

/**
 *	Let and LED blink for <ms> on pin <pin> and port <prt>
 */
void blinkLED(volatile uint8_t * const prt, uint8_t pin, uint16_t ms);

#endif