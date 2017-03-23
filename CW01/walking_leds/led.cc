// Program zaswieca pojedyncza diode LED w czasie nacisniecia przycisku.
//
// Wymagane polaczenia w ukladzie ZL15AVR:
//        PA0 - LED 0
//        PA1 - SW0
//
// Autor: Pawel Klimczewski, 15 stycznia 2010.

#include <avr/io.h>
#include <util/delay.h>

//---------------------------------------------------------------------------
void configure_pins()
{
  // Ustaw wyprowadzenie PA0 w tryb wyjscia.
  DDRA = 255; 
  // Wyprowadzenie PA1 jest poczatkowo ustawione jako wejsciowe
  // bez rezystora podciagajacego, zatem dodatkowa konfiguracja nie jest
  // potrzebna.
}
//---------------------------------------------------------------------------
int main()
{
  configure_pins();
  int dir = 0;
  int A = 1;
  int B = 128;
  while ( true )
  {
	if(dir == 0)
	{
		A = A << 1;
		B = B >> 1;
	}
	else
	{
		A = A >> 1;
		B = B << 1;
	}
	if(A == 1)
	{
		dir = 0;
	}
	if(A == 128)
	{
		dir = 1;
	}
	PORTA = A|B;
	_delay_ms(200);
  }
  return 0;
}
//---------------------------------------------------------------------------
