#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>
#include "biblio.h"

// Komunikacja z ukladem termometru tc77.
//
// Wymagane polaczenia w ukladzie ZL15AVR:
//
//  U7 (TEM) CS  - PB4
//  U7 (TEM) SIO - PB6  
//  U7 (TEM) SCK - PB7
//  TxD - PD1
//
// Autor: Pawel Klimczewski, 19 marca 2012.

inline void set_bit( volatile uint8_t& reg, uint8_t bit, bool b )
{
  if ( b )
    reg |= 1 << bit;
  else
    reg &= ~( 1 << bit );
}

// Poczatek opisu podlaczenia ukladu tc77.

void configure_ctrl_lines()
{
  DDRB  |=    1 << PB4 |            1 << PB7  ;
  PORTB &= ~( 1 << PB4 | 1 << PB6 | 1 << PB7 );
}

void configure_sio_as_input()
{
  set_bit( DDRB, PB6, false );
}

void configure_sio_as_output()
{
  set_bit( DDRB, PB6, true );
}

void cs ( bool b ) { set_bit( PORTB, PB4, b ); }
void sck( bool b ) { set_bit( PORTB, PB7, b ); }
void sio( bool b ) { set_bit( PORTB, PB6, b ); }

bool sio()
{
  return PINB & 1 << PB6;
}

// Koniec opisu podlaczenia ukladu tc77.

uint16_t read16()
{
  uint16_t value = 0;
  for ( uint8_t i = 0; i < 16; ++i )
  {
    value <<= 1;
    value |= sio();
    sck( true );
    sck( false );
  }
  return value;
}

void write16( uint16_t value )
{
  configure_sio_as_output();
  for ( uint8_t i = 0; i < 16; ++i )
  {
    sio( value & 0x8000 );
    value <<= 1;
    sck( true );
    sck( false );
  }
  configure_sio_as_input();
}

int main()
{
  usart( stdout );
  configure_ctrl_lines();
  configure_sio_as_input();
  printf( "Termometr\n" );
  while ( true )
  {
    // Sygnalizacja rozpoczecia transmisji.
    cs( false );
    // Odczytanie temperatury (bity 15-3) i bitu kontrolnego (2).
    uint16_t value1 = read16();
    // Przelaczenie ukladu tc77 do trybu "shutdown mode".
    write16( 0x00ff );
    // Sygnalizacja zakonczenia transmisji.
    cs( true );

    printf( "0x%04x = %d+%d/16 C\n", value1, value1 >> 7, value1 >> 3 & 0x0f );
        
    _delay_ms( 1 );
    
    // Sygnalizacja rozpoczecia transmisji.
    cs( false );
    // Odczytanie identyfikatora ukladu tc77.
    uint16_t value2 = read16();
    // Przelaczenie ukladu tc77 do trybu pomiarow.
    write16( 0x0000 );
    // Sygnalizacja zakonczenia transmisji.
    cs( true );
        
    printf( "0x%04x\n", value2 );
    
    _delay_ms( 1000 );
  }
  return 0;
}