// Program zaswieca pojedyncza diode LED w czasie nacisniecia przycisku.
//
// Wymagane polaczenia w ukladzie ZL15AVR:
//        PA0 - LED 0
//
// Autor: Pawel Klimczewski, 15 stycznia 2010.
// Autor: Marcin Polkowski , 16 marca 2017.

#include <avr/io.h>
#include <util/delay.h>

//---------------------------------------------------------------------------
void configure_pins()
{
  // Ustaw wyprowadzenie PA0 w tryb wyjscia.
  DDRA = 1 << PA0; 
  // Wyprowadzenie PA1 jest poczatkowo ustawione jako wejsciowe
  // bez rezystora podciagajacego, zatem dodatkowa konfiguracja nie jest
  // potrzebna.
}
//---------------------------------------------------------------------------
int main()
{
	configure_pins();
	int d = 0;
	while(true)
	{
		// Dioda mruga
		// UWAGA: rozwiazanie prymitywne
		PORTA |= 1 << PA0;
		_delay_ms(d);
		PORTA &= ~( 1 << PA0 ); 
		_delay_ms(d);
		
		d = d + 5;
		if(d > 250)
		{
			d = 0;
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
