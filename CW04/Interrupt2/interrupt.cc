#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "biblio.h"

// Obserwacja wskaznika stosu podczas wykonywania przerwan.
//
//  PD2 - SW0
//
//  LCD TEXT RW - GND
//  LCD TEXT RS - PC2
//  LCD TEXT E  - PC3
//  LCD TEXT D4 - PC4
//  LCD TEXT D5 - PC5
//  LCD TEXT D6 - PC6
//  LCD TEXT D7 - PC7
//
// Autor: Pawel Klimczewski, 6 marca 2010.

static volatile int n = 0;

ISR( INT0_vect)//, ISR_NOBLOCK )

{
  //cli();
  ++n;
  printf( "\an=%d SP=%d\n", n, SP );
  
  _delay_ms( 250 );
  //sei();
}

int main()
{
  hd44780( stdout, PORTC );
  printf( "\nSP=%d\n", SP );
  MCUCR |= 1 << ISC01 | 1 << ISC00; // Kazda zmiana napiecia generuje przerwanie.
  GICR |= 1 << INT0;   // Uaktywniamy przerwanie.
  sei();               // Wlaczamy obsluge przerwan.
  while ( true );
  return 0;
}