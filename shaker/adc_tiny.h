/**
 *	Analog/digital Converter helper functions.
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

#ifndef adc_tiny_h
#define adc_tiny_h

#include <avr/io.h>

/*------------*/
/* ADC CONFIG */
/*------------*/

#ifndef ADC_MASK_AREF
#define ADC_MASK_AREF 0x00	// ext. AREFs
#endif
#ifndef ADC_MASK_PRESCALER
//#define ADC_MASK_PRESCALER (1<<ADPS2) | (1<<ADPS1)	// for 10Mhz it must be between 50 and 200 -> set it to 64
#define ADC_MASK_PRESCALER (1<<ADPS2)	// for 1Mhz it must be between 5 and 20 -> set it to 16
#endif

/*-----------*/
/* FUNCTIONS */
/*-----------*/

/**
 *	Initializes the ADC with the above settings
 */
void adc_init();

/**
 *	Reads a value from the ADC channel <chan>
 */
uint16_t adc_read(uint8_t chan);

/**
 *	Reads <samples> times a value from the ADC channel <chan> and
 *	returns the average
 */
uint16_t adc_read_avg(uint8_t chan, uint8_t samples);

#endif