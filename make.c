#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/io.h>
#include <util/delay.h>
#INCLUDE <avr/io.h>
#define F_CPU 1000000

int main(void) {

	DDRD = 0x0F;
	PORTC = 0x00;
	
	a=0b1010;
	b=0b0110;
	c=0b0101;
	d=0b1001;
	
	while(1) {
	PORTC=a;
	PORTC=b;
	PORTC=c;
	PORTC=d;
	}