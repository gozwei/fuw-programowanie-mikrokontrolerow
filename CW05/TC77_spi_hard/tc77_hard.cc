#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include "biblio.h"

// Komunikacja z ukladem termometru tc77 z wykorzystaniem ukladu odbiornika
// SPI.
//
// Wymagane polaczenia w ukladzie ZL15AVR:
//
//  U7 (TEM) CS  - PB4
//  U7 (TEM) SIO - PB6
//  U7 (TEM) SCK - PB7
//  TxD - PD1
//
// Autor: Pawel Klimczewski, 14 marca 2010.


// Poczatek opisu podlaczenia ukladu tc77.

void spi_init()
{
  DDRB |= 1 << PB7 | 1 << PB4;
  // Uklad odbiornika SPI pracuje w trybie "master". Czestotliwosc zegara
  // F_CPU/16.
  SPCR = 1 << SPE | 1 << MSTR | 1 << SPR0;
}

uint8_t spi_transmit( uint8_t v )
{
  SPDR = v;
  while ( !( SPSR & 1 << SPIF ) );
  return SPDR;
}

void cs( bool b )
{ 
  if ( b )
    PORTB |=    1 << PB4  ;
  else
    PORTB &= ~( 1 << PB4 );
}

// Koniec opisu podlaczenia ukladu tc77.

uint16_t read16()
{
  uint16_t value = spi_transmit( 0 );
  value <<= 8;
  value |= spi_transmit( 0 );
  return value;
}

int main()
{
  usart( stdout );
  spi_init();
  while ( true )
  {
    // Sygnalizacja rozpoczecia transmisji.
    cs( false );
    // Odczytanie temperatury (bity 15-3) i bitu kontrolnego (2).
    uint16_t value = read16();
    // Sygnalizacja zakonczenia transmisji.
    cs( true );
    
    if ( value & 0x0004 )
    {
      value >>= 3;
      // Formatowanie zmiennopozycyjne wymaga dodatkowych opcji konsolidacji
      //   -Wl,-u,vfprintf -lprintf_flt -lm
      // i znaczaco zwieksza rozmiar programu.
      printf( "%.1f\n", value / 16. );
    }
    else
    {
      printf( "czekaj\n" );
    }
    
    _delay_ms( 1000 );
  }
  return 0;
}