   // Program generuje przebieg prostokatny na wyjsciu PA0.
//
// Autor: Pawel Klimczewski, 1 lutego 2010.

#include <avr/io.h>
#include <util/delay.h>

int digits[10] = {63, 6, 91, 79, 102, 109, 125, 7, 127, 111};
int seg[4] = {1 << PD3, 1 << PD2, 1 << PD1, 1 << PD0};
int buf[4] = {0,0,0,0};

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
  int t = 0;
  PORTD = 255;
  while ( true )
  {
	t++;
	PORTD = 255;
	PORTA = ~digits[buf[x]];
	if(x==2)
	{
		PORTA &= ~(1 << PA7);
	}
	PORTD = ~seg[x];
	x++;
	if(x > 3)
	{
		x = 0;
	}
	if(t>1000)
	{
		t = 0;
		buf[3]++;
		if(buf[3]>9)
		{
			buf[2]++;
			buf[3]=0;
		}
		if(buf[2]>9)
		{
			buf[1]++;
			buf[2]=0;
		}
		if(buf[1]>9)
		{
			buf[0]++;
			buf[1]=0;
		}
		if(buf[0]>9)
		{
			buf[0]=0;
		}
	}
	_delay_us(100);
	
	
	
  }
  return 0;
}
//---------------------------------------------------------------------------
