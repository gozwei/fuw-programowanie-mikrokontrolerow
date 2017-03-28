#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include "biblio.h"

// Program rozpoznaje krotkie i dlugie (powyzej sekundy) nacisniecie
// przycisku.
//
// Wymagane polaczenia w ukladzie ZL15AVR:
//
//  PB0 - SW0
//  PD1 - TxD
//
// Autor: Pawel Klimczewski, 12 marca 2012.

static uint8_t t = 0;

#define T1S (61)

uint8_t volatile krotko, dlugo;

ISR( TIMER2_OVF_vect )
{
  if ( ( PINB & 1 << PB0 ) == 0 )
  {
    // Przycisk jest nacisniety. Jezeli akurat uplynela sekunda od chwili
    // wcisniecia to klasyfikujemy jako "dlugie" nacisniecie.
    if ( t != T1S && ++t == T1S ) 
      ++dlugo;
  }
  else
  {
    // Przycisk nie jest nacisniety. Jezeli akurat zakonczylo sie
    // nacisniecie trwajace ponizej 1 sekundy to klasyfikujemy jako
    // "krotkie" nacisniecie.
    if ( t != 0 && t != T1S )
      ++krotko;
    t = 0;
  }
}

void configure_timer()
{
  // Licznik 2 zlicza z czestotliwoscia F_CPU / 1024 = 15 625 Hz
  // Przepelnienie nastepuje co 16 ms (61 Hz) i generuje przerwanie
  // TIMER2_OVF.
  TCCR2 = 1 << CS22 | 1 << CS21 | 1 << CS20;
  TIMSK |= 1 << TOIE2;  
}
  
int main()
{
  usart( stdout );
  configure_timer();
  sei();
  printf( "nacisnij SW0\n\n" );
  int16_t n = 0;
  while ( true )
  {
    if ( krotko )
    {
      printf( "%d krotko\n", ++n );
      --krotko;
    }
    if ( dlugo )
    {
      printf( "%d dlugo\n", ++n );
      --dlugo;
    }
  }
  return 0;
}
