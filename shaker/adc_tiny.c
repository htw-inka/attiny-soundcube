#include "adc_tiny.h"

#include "tools_tiny.h"

void adc_init() {
  uint16_t result;
  
  ADMUX = ADC_MASK_AREF;
  
  ADCSRA = ADC_MASK_PRESCALER;          // Prescaler Select Bits
  ADCSRA |= (1<<ADEN);                  // ADC activate
 
  // dummy readout
  ADCSRA |= (1<<ADSC);                  // start conversion
  while (ADCSRA & (1<<ADSC) ) {         // wait until finished
  }
  
  // read out once
  result = ADCW;   
}

uint16_t adc_read(uint8_t chan) {
  // choose channel
  ADMUX = (ADMUX & ~(0x1F)) | (chan & 0x1F);
  
  ADCSRA |= (1<<ADSC);              // start conversion
  while (ADCSRA & (1<<ADSC)) {      // wait until finished
  }
  
  return ADCW;                    // return the read value
}

uint16_t adc_read_avg(uint8_t chan, uint8_t samples) {
  uint32_t sum = 0;
  uint8_t i = 0;

  for (; i < samples; ++i) {
    sum += adc_read(chan);
  }
 
  return (uint16_t)(sum / samples);
}