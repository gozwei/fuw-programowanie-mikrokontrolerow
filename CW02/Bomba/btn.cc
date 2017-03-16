// Program zaswieca pojedyncza diode LED w czasie nacisniecia przycisku.
//
// Wymagane polaczenia w ukladzie ZL15AVR:
//        PA0 - LED 0
//        PA1 - SW0
//
// Autor: Pawel Klimczewski, 15 stycznia 2010.

#include <avr/io.h>

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
  while ( true )
  {
    if ( PINA & 1 << PA1 )
    {
      // Na wyprowadzeniu PA1 mamy stan wysoki. Przycisk nie jest nacisniety.
      PORTA &= ~( 1 << PA0 ); // Wylaczam diode.
    }
    else
    {
      // Na wyprowadzeniu PA1 mamy stan niski. Przycisk jest nacisniety.
      PORTA |= 1 << PA0; // Wlaczam diode.
    }
  }
  return 0;
}
//---------------------------------------------------------------------------
