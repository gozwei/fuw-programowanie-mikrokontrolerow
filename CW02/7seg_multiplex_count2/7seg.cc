#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include <stdlib.h>


// Rozwi¹zanie Damiana W³odzyñskiego (2017-03-23)


void configure_pins()
{
  DDRA = 0xff;
  DDRB = 0x0f;
}
uint8_t digits[ 10 ] = { 63, 6, 91, 79, 102, 109, 125, 7, 127, 111 };
uint8_t seg[ 4 ] = { 1 << PB3, 1 << PB2, 1 << PB1, 1 << PB0 };
uint16_t a[4] = {1000,100,10,1};
uint32_t t = 0;

int main()
{
  configure_pins();
  PORTA = 0xff;
  while ( true )
  {
	PORTA = ~digits[ (t/100/a[t%4])%10 ];
	if((t%4)==2)PORTA &= ~(1<<PA7);
	PORTB = ~seg[ t%4 ];
	t++;
	if(t==1000000)t=0;
	_delay_ms( 1 );
  }
  return 0;
}