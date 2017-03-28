#include <avr/io.h>
#include <util/delay.h>
#include <inttypes.h>

// Programowa realizacja transmisji szeregowej w formacie 8N1.
//
// Sposob podlaczenia w ukladzie ZL15AVR.
//
// RXD = PD0
// TXD = PD1
//
// Pawel Klimczewski, 5 marca 2012.

#define BAUDRATE ( 1200 )
#define DT_US ( 1000000. / BAUDRATE )
//---------------------------------------------------------------------------
void configure_pins()
{
  DDRD |= 1 << PD1;
}
//---------------------------------------------------------------------------
inline void tx_bit( bool b )
{
  if ( b )
    PORTD |= 1 << PD1;
  else
    PORTD &= ~( 1 << PD1 );
}
//---------------------------------------------------------------------------
inline uint8_t rx_bit()
{
  return ( PIND & 1 << PD0 ) ? 1 : 0;
}
//---------------------------------------------------------------------------
void tx( uint8_t b )
{
  // Bit startu.
  tx_bit( false );
  _delay_us( DT_US );
  // 8 bitow danych.
  for ( uint8_t n = 8; n; --n )
  {
    tx_bit( b & 1 );
    b >>= 1;
    _delay_us( DT_US );
  }
  // Bit stopu.
  tx_bit( true );
  _delay_us( DT_US );
}
//---------------------------------------------------------------------------
uint8_t rx()
{
  // Czekamy na opadajace zbocze sygnalu.
  loop_until_bit_is_clear( PIND, PD0 );
  _delay_us( 1.5 * DT_US );
  // Odczytujemy 8 bitow danych.
  uint8_t b = 0;
  for ( uint8_t n = 0; n < 8; ++n )
  {
    b |= rx_bit() << n;
    _delay_us( DT_US );    
  }
  // Czekamy na narastajace zbocze sygnalu.
  loop_until_bit_is_set( PIND, PD0 );
  return b;
}
//---------------------------------------------------------------------------
int main()
{
  configure_pins();
  tx_bit( true ); 
  const char* p="Hello world!\r\n";
  for ( ; *p; ++p )
  {
    tx( *p );
    _delay_ms( 500 );
  }
  while ( true )
  {
    tx( rx() - 32);
  }
  return 0;
}
//---------------------------------------------------------------------------
