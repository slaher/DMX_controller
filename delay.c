# include "delay.h"

void Delay(unsigned int us) // opóŸnienie w mikrosekundach us -> delay
{
    unsigned int delay_loops;
    register unsigned int  i;
    delay_loops = (us+3)/5*CYCLES_PER_US; // +3 for rounding up (dirty)
    for (i=0; i < delay_loops; i++) {};
} 