#include <avr/io.h>
#include <avr/interrupt.h>
#include "biblio.h"

// Obsluga przerwania INT0.
//
// Wymagane polaczenia w ukladzie ZL15AVR:
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

ISR( INT0_vect, ISR_NOBLOCK )
{
  ++n;
}

int main()
{
  hd44780( stdout, PORTC );
  MCUCR |= 1 << ISC00; // Kazda zmiana napiecia generuje przerwanie.
  GICR |= 1 << INT0;   // Uaktywniamy przerwanie.
  sei();               // Wlaczamy obsluge przerwan.
  while ( true )
  {
    printf( "%d\n\n", n );
  }
  return 0;
}