#include <avr/pgmspace.h>
#include <util/delay.h>
#include "hd44780.h"

// "Hello world" wyswietlacza alfanumerycznego.
//
// Autor: Pawel Klimczewski, 2 marca 2009.

int main()
{
  hd44780_initialize();
  hd44780_cursor_on();
  hd44780_blinking_on();
  uint8_t n = 16;
  const char* p =
    "Programowanie mikrokontrolerow AVR w jezyku C jest proste i przyjemne. Korzystamy ze srodowiska GNU GCC oraz ukladu ZL15AVR.";
  for ( ; *p; ++p )
  {
    hd44780_write_data( *p );
    if ( !--n )
    {
      hd44780_auto_scroll();
    }
    _delay_ms( 25 );
  }
  return 0;
}