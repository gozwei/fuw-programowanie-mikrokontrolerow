#include <avr/io.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <inttypes.h>
#include "biblio.h"

// Przyklad uzycia ukladu automatycznego zerowania "watchdog".
//
// Wymagane polaczenia w ukladzie ZL15AVR:
//
//  LCD TEXT RW - GND
//  LCD TEXT RS - PC2
//  LCD TEXT E  - PC3
//  LCD TEXT D4 - PC4
//  LCD TEXT D5 - PC5
//  LCD TEXT D6 - PC6
//  LCD TEXT D7 - PC7
//
//  SW0 - PA0
//
// Autor: Pawel Klimczewski, 22 marca 2010.

int main()
{
  // Wlaczam uklad automatycznego zerowania po okresie 2 sekund.
  wdt_enable( WDTO_2S );
  //hd44780( stdout, PORTC );
  usart(stdout);
  for ( uint8_t i = 0; ; ++i )
  {
    printf( "%d\n", i );
    for ( uint8_t n = 100; n; --n )
    {
      if ( ( PINA & 1 << PA0 ) == 0 )
      {
        // W czasie kiedy jest nacisniety przycisk zeruje licznik ukladu
        // watchdog.
        wdt_reset();
      }
      _delay_ms( 10 );
    }
  }
  return 0;
}
