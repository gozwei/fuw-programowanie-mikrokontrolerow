   // Program generuje przebieg prostokatny na wyjsciu PA0.
//
// Autor: Pawel Klimczewski, 1 lutego 2010.

#include <avr/io.h>
#include <util/delay.h>

int digits[10] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};
int seg[4] = {1 << PD3, 1 << PD2, 1 << PD1, 1 << PD0};
int buf[4] = {2,0,1,7};

//---------------------------------------------------------------------------
void configure_pins()
{
  DDRA = 255;
  DDRD = 255;
}
//---------------------------------------------------------------------------
int main()
{
  configure_pins();
  int x = 0;
  PORTD = 255;
  while ( true )
  {
	PORTD = 255;
	PORTA = ~digits[buf[x]];
	PORTD = ~seg[x];
	x++;
	if(x > 3)
	{
		x = 0;
	}
	_delay_us(1000);
	
	
	
  }
  return 0;
}
//---------------------------------------------------------------------------
