#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

// Odtwarzacz melodii.
//
// Wymagane polaczenia w ukladzie ZL15AVR:
//
//  PD5 - SPK
//
// Autor: Pawel Klimczewski, 8 marca 2010.

//---------------------------------------------------------------------------
void configure_pins()
{
  DDRD |= 1 << PD5; // Wyjscie OC1A  
}
//---------------------------------------------------------------------------
void configure_timers()
{
  // Licznik 0 zlicza z czestotliwoscia F_CPU / 1024 = 15 625 Hz
  // Przepelnienie nastepuje co 16 ms (61 Hz) i generuje przerwanie
  // TIMER0_OVF.
  TCCR0 = 1 << CS02 | 1 << CS00;
  TIMSK = 1 << TOIE0;
  
  // Licznik jeden sluzy do generowania fali prostokatnej.
  // Zmiana napiecia na wyprowadzeniu na przeciwne.
  TCCR1A = 1 << COM1A0;  
}
//---------------------------------------------------------------------------
void modulacja( uint16_t dt )
{
  if ( dt == 0 )
  {
    TCCR1B = 0; // Wylaczam licznik.    
  }
  else
  {
    // Zeruje licznik.
    TCNT1 = 0;
    // Ustawiam zakres licznika aby odpowiadal biezacej nucie.
    OCR1A = dt;
    // Wlaczam licznik 1.
    // Licznik zlicza z czestotliwoscia F_CPU / 8 = 2 MHz.
    TCCR1B = 1 << WGM12 | 1 << CS11;
  }
}
//---------------------------------------------------------------------------
#define PAUZA 0xff

// Tablica przedstawia fragment uwtoru "Dla Elizy" Ludwika van Beethovena.
// Pojedynczy dzwiek reprezentowany jest przez pare wartosci
// ( wysokosc dzwieku, dlugosc dzwieku ).
//
// Wysokosc dzwieku % 12 jest indeksem tablicy nuty.
//
// Dlugosci dzwiekow: 32 - cala nuta, 16 - polnuta, 8 - cwiercnuta,
// 4 - osemka.

uint8_t melodia[] =
{
                                          12+ 4, 4, 12+ 3, 4, 
  12+ 4, 4, 12+ 3, 4, 12+ 4, 4,    11, 4, 12+ 2, 4, 12+ 0, 4, 
      9, 8,           PAUZA, 4,     0, 4,     4, 4,     9, 4,
     11, 8,           PAUZA, 4,     4, 4,     8, 4,    11, 4,
  12+ 0, 8,           PAUZA, 4,     4, 4, 12+ 4, 4, 12+ 3, 4,
  12+ 4, 4, 12+ 3, 4, 12+ 4, 4,    11, 4, 12+ 2, 4, 12+ 0, 4,
      9, 8,           PAUZA, 4,     0, 4,     4, 4,     9, 4,
     11, 8,           PAUZA, 4,     4, 4, 12+ 0, 4,    11, 4,
      9, 8,           PAUZA, 4,
  
     11, 4, 12+ 0, 4, 12+ 2, 4,
  12+ 4,12,                         7, 4, 12+ 5, 4, 12+ 4, 4,
  12+ 2,12,                         5, 4, 12+ 4, 4, 12+ 2, 4,
  12+ 0,12,                         4, 4, 12+ 2, 4, 12+ 0, 4,
     11, 8,           PAUZA, 4,     4, 4, 12+ 4, 4, PAUZA, 4,
  PAUZA, 4, 12+ 4, 4, 24+ 4, 4, PAUZA, 8,           12+ 3, 4,
  12+ 4, 4, PAUZA, 8,           12+ 3, 4, 12+ 4, 4, 12+ 3, 4,
  12+ 4 ,4, 12+ 3, 4, 12+ 4, 4,    11, 4, 12+ 2, 4, 12+ 0, 4,
      9, 8,           PAUZA, 4,     0, 4,     4, 4,     9, 4,

     11, 8,           PAUZA, 4,     4, 4,     8, 4,    11, 4,
  12+ 0, 8,           PAUZA, 4,     4, 4, 12+ 4, 4, 12+ 3, 4,
  12+ 4, 4, 12+ 3, 4, 12+ 4, 4,    11, 4, 12+ 2, 4, 12+ 0, 4,
      9, 8,           PAUZA, 4,     0, 4,     4, 4,     9, 4,
     11, 8,           PAUZA, 4,     4, 4, 12+ 0, 4,    11, 4,
      9, 8,           PAUZA, 4,
  
  PAUZA,64
};

// Licznik 1 zliczajacy z czestotliwoscia 2 MHz i zmieniajacy napiecie
// na wyjsciu mikrokontrolera na przeciwne po zliczeniu podanej liczby
// impulsow generuje dzwiek o odpowiadajacej wysokosci.

uint16_t nuty[] =
{
  3823, // c   0
  3607, // c#  1
  3405, // d   2
  3214, // d#  3
  3034, // e   4
  2864, // f   5
  2703, // f#  6
  2551, // g   7
  2408, // g#  8
  2273, // a   9
  2145, // a# 10
  2025  // h  11
};
//---------------------------------------------------------------------------
uint16_t volatile idx = 0;
uint16_t volatile czas = 1;

ISR( TIMER0_OVF_vect )
{
  --czas;
  if ( czas == 2 )
  {
    // Wprowadzam krotkie przerwy pomiedzy poszczegolnymi nutami.
    modulacja( 0 );
  }
  if ( czas )
  {
    // Jeszcze nie uplynal czas trwania nuty.
    return;
  }
  uint8_t nuta = melodia[ idx++ ];
  czas = melodia[ idx++ ];
  czas <<= 2;
  if ( idx == sizeof( melodia ) )
  {
    idx = 0;
  }
  if ( nuta == PAUZA )
  {
    modulacja( 0 );
  }
  else
  {
    int8_t oktawa = 1;
    while ( nuta >= 12 )
    {
      nuta -= 12;
      oktawa++;
    }
    modulacja( nuty[ nuta ] >> oktawa );
  }
}
//---------------------------------------------------------------------------
int main()
{
  configure_pins();
  configure_timers();
  sei();
  while ( true );
  return 0;
}
//---------------------------------------------------------------------------