// Program zaswieca pojedyncza diode LED w czasie nacisniecia przycisku.
//
// Wymagane polaczenia w ukladzie ZL15AVR:
//        PA0 - LED 0
//        PA1 - SW0
//        PA2 - SW1
//        PA3 - SW2
//        PA4 - SW3
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
  
  int bomba = 1;
  // 1 => bomba uzbrojona
  // 2 => bomba rozbrojona
  // 3 => eksplozja
  
  while ( true )
  {
	if ( !(PINA & 1 << PA2) )
	{
		if (bomba==1)
		{
			bomba = 2;
		}
	}
	
	if ( !(PINA & 1 << PA1) )
	{
		if (bomba!=2)
		{
			bomba = 3;
		}
	}
	
	if ( !(PINA & 1 << PA3) )
	{
		if (bomba!=2)
		{
			bomba = 3;
		}
	}
	
	if ( !(PINA & 1 << PA4) )
	{
		if (bomba!=2)
		{
			bomba = 3;
		}
	}
	

    if (bomba==2)
    {
		PORTA |= 1 << PA0; // Dioda swieci
    }
    else if (bomba == 3)
    {
		// Dioda mruga
		// UWAGA: rozwiazanie prymitywne
		PORTA |= 1 << PA0;
		_delay_ms(50);
		PORTA &= ~( 1 << PA0 ); 
		_delay_ms(50);
    }
	else
	{
		PORTA &= ~( 1 << PA0 ); // Dioda nie swieci
	}
  }
  return 0;
}
//---------------------------------------------------------------------------
