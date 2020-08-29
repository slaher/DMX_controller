#include "avr/io.h"
#include "inttypes.h"
#include "stdio.h"
#include "util/delay.h"
#include "avr/interrupt.h"
#include "usart.h"
#include "avr/wdt.h"
#include <avr/eeprom.h>

void Delay(unsigned int us);