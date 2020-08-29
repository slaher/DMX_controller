#include "avr/io.h"
#include "inttypes.h"
#include "stdio.h"
#include "util/delay.h"
#include "avr/interrupt.h"
#include "usart.h"
#include "avr/wdt.h"
#include <avr/eeprom.h>

unsigned int Read_adc(unsigned char adc_input);