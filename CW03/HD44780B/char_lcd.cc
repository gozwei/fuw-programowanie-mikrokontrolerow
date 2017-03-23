#include <avr/pgmspace.h>
#include <util/delay.h>
#include "hd44780.h"

// Wykorzystanie pamieci programu do zapamietania napisu.
//
// Autor: Pawel Klimczewski, 2 marca 2009.

int main()
{
  hd44780_initialize();
  hd44780_cursor_on();
  hd44780_blinking_on();
  uint8_t n = 16;
  prog_char* src = PSTR(
    "Programowanie mikrokontrolerow AVR w jez"
    "yku C jest proste i przyjemne. Korzystam"
    "y ze srodowiska GNU GCC oraz ukladu ZL15"
    "AVR." );
  for ( ; pgm_read_byte( src ); ++src )
  {
    hd44780_write_data( pgm_read_byte( src ) );
    if ( !--n )
    {
      hd44780_auto_scroll();
    }
    _delay_ms( 250 );
  }
  return 0;
}