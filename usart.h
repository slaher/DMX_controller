/*
 * usart.h
 *
 *  Created on: 27-jan-2009
 *      Author: Jan
 */

#ifndef USART_H_
#define USART_H_

void USART_Init(unsigned int baudrate)
{
	/* Set baud rate */
	UBRRH = (unsigned char)(baudrate>>8);
	UBRRL = (unsigned char)baudrate;

	/* Enable receiver and interrupt*/
//	UCSRB = (1<<RXEN)|(1<<RXCIE)|(1<<TXEN);
UCSRB &= ~(1<<RXEN);
UCSRB &= ~(1<<TXEN);
	/* Set frame format: 9data UCSZ0 = 7, 8data UCSZ0 = 3, 1stop bit */
	UCSRC = (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1)|(0<<UCSZ2)|(1<<USBS);
}

void USART_Transmit( unsigned char data )
{
/* Wait for empty transmit buffer */
while ( !( UCSRA & (1<<UDRE)) )
;
/* Put data into buffer, sends the data */
UDR = data;
}

unsigned char USART_Receive( void )
{
unsigned char status, resl;
/* Wait for data to be received */
while ( !(UCSRA & (1<<RXC)) )
;
/* Get status and 9th bit, then data */
/* from buffer */
status = UCSRA;
resl = UDR;
/* If error, return -1 */
if ( status & (1<<FE))
return -1;
/* Filter the 9th bit, then return */
return (resl);
}

#endif /* USART_H_ */
