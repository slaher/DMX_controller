
#define CYCLES_PER_US ((F_CPU+500000)/1000000) // cpu cycles
#define ADC_VREF_TYPE 0xE0


//Includes
#include "avr/io.h"
#include "inttypes.h"
#include "stdio.h"
#include "util/delay.h"
#include "avr/interrupt.h"
#include "usart.h"
#include "avr/wdt.h"
#include <avr/eeprom.h>


int data[520]={0};
 

void delay(unsigned int us) // delay [us]
{
    unsigned int delay_loops;
    register unsigned int  i;
    delay_loops = (us+3)/5*CYCLES_PER_US; // +3 for rounding up (dirty)
    for (i=0; i < delay_loops; i++) {};
} 


unsigned int read_adc(unsigned char adc_input) {
 ADMUX=adc_input|ADC_VREF_TYPE; 
 ADCSRA|=0x40; 
 while ((ADCSRA & 0x10)==0); 
 ADCSRA|=0x10; 
 return ADCH; 
 }

unsigned int start_address()
{
	int address = 0;
	//set address switch ports
	DDRC = 0x00;
	PORTC = 0xff;
	DDRD = 0x7F;
	PORTD = 0x80;
	
	// read address
	if(!(PIND & (1<<PD7))) {address = address + 1;}
	else {address = address;}
	if(!(PINC & (1<<PC0))) {address = address + 2;}
	else {address = address;}
	if(!(PINC & (1<<PC1))) {address = address + 4;}
	else {address = address;}
	if(!(PINC & (1<<PC2))) {address = address + 8;}
	else {address = address;}
	if(!(PINC & (1<<PC3))) {address = address + 16;}
	else {address = address;}
	if(!(PINC & (1<<PC4))) {address = address + 32;}
	else {address = address;}
	if(!(PINC & (1<<PC5))) {address = address + 64;}
	else {address = address;}
	if(!(PINC & (1<<PC6))) {address = address + 128;}
	else {address = address;}
	if(!(PINC & (1<<PC7))) {address = address + 256;}
	else {address = address;}
	
	return address;
}

void alarm(int program, int address)
{
	switch (program)
	{
		case 1:
			data[0 + address] = 0x00;
			data[1 + address] = 0x00;
			data[2 + address] = 0xFF;
			data[3 + address] = 0x00;
			data[4 + address] = 0x00;				
			data[5 + address] = 0x00;
			_delay_us(30);
			break;
			
		case 2:
			data[0 + address] = 0x00;
			data[1 + address] = 0x00;
			data[2 + address] = 0x00;
			data[3 + address] = 0xFF;
			data[4 + address] = 0x00;				
			data[5 + address] = 0x00;
			_delay_us(30);
			break;
		
		case 3:
			data[0 + address] = 0x00;
			data[1 + address] = 0x00;
			data[2 + address] = 0x00;
			data[3 + address] = 0x00;
			data[4 + address] = 0xFF;				
			data[5 + address] = 0x00;
			_delay_us(30);
			break;	
			
		case 4:
			data[0 + address] = 0x00;
			data[1 + address] = 0x00;
			data[2 + address] = 0xFF;
			data[3 + address] = 0xFF;
			data[4 + address] = 0xF;				
			data[5 + address] = 0x00;
			_delay_us(30);
			break;
			
		case 5:
			//odczyt ADC
			data[0 + address] = read_adc(0);
			data[1 + address] = read_adc(1);
			data[2 + address] = read_adc(2);
			data[3 + address] = read_adc(3);
			data[4 + address] = read_adc(4);
			data[5 + address] = read_adc(5);
			data[6 + address] = read_adc(6);
			data[7 + address] = read_adc(7);
			break;
	}
		
		
}


int main(){

	int i = 0;
	int address = 0;
	int program = 0;
	_delay_ms(500); 		// wait for stabilize input voltage
	

	// alarm inputs
	DDRB = 0x00;
	PORTB = 0xFF;
	
	// configure ADC
	ACSR = 0x80; 
	SFIOR = 0x00; 
	ADMUX = ADC_VREF_TYPE; 
	ADCSRA = 0x86;
	
	USART_Init(3);								// USART init (250Kbps, 8 databits, 2 stopbits)
	wdt_disable();								// disable watchdoga
	
	address = start_address();
	
	// blinking LED if address not set
	if (address == 0)
	{
		while(1)
		{
			DDRD = 0xFF;
			PORTD = 0x00;
			_delay_ms(500);
			_delay_ms(500);
			PORTD = 0xFF;
			_delay_ms(500);
			_delay_ms(500);
		}
	}
	
	

///////////////////////  MAIN DMX LOOP //////////////////

	
	while(1){
		UCSRB &= ~(1<<TXC)&~(1<<TXEN);
		PORTD = 0X00;
		
			
		if(!(PINB & (1<<PB1))) { program = 1; }	
		
		else if(!(PINB & (1<<PB2))) { program = 2; }
		
		else if(!(PINB & (1<<PB3))) { program = 3; }
		
		else if(!(PINB & (1<<PB4))) { program = 4; }
				
		else { program = 5; }
		
		alarm(program, address);
		
		
		_delay_us(88);
		PORTD = 0x0F;
		_delay_us(8);
		UCSRB |= (1<<TXC)|(1<<TXEN);
		
		USART_Transmit(0);
		
		_delay_us(8);
			
		for(i=1;i<=(address+6) && i<512;i++)
			{
				USART_Transmit(data[i]);
				_delay_us(8);
			}
	
		while(!(UCSRA & (1<<TXC)));
		UCSRB &= ~(1<<TXC)&~(1<<TXEN);
		PORTD = 0x0F;
		delay(20);
	
	}
}