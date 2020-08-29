#include "read_adc.h"

unsigned int Read_adc(unsigned char adc_input) {
 ADMUX=adc_input|ADC_VREF_TYPE; 
 ADCSRA|=0x40; 
 while ((ADCSRA & 0x10)==0); 
 ADCSRA|=0x10; return ADCH; 
 }
