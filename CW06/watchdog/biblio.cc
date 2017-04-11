#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "biblio.h"

#ifndef UART_SPEED
#define UART_SPEED 9600
#endif // UART_SPEED

// Implementacja standardowego strumienia wyjsciowego stdout na wyswietlaczu
// alfanumerycznym LCD, 2 wiersze po 16 znakow, zgodnym z HD44780.
//
// Implementacja standardowego strumienia diagnostycznego stderr do portu
// RS232, UART_SPEED bps, 8N1.
//
// Autor: Pawel Klimczewski, 4 marca 2009.
//
// Sposob podlaczenia lacza RS232.
//
// RXD = PD0
// TXD = PD1
//
// Sposob podlaczenia wyswietlacza LCD.
//
// RW = GND
// RS = PX2
// E  = PX3
// D4 = PX4
// D5 = PX5
// D6 = PX6
// D7 = PX7

//---------------------------------------------------------------------------
// Opis podlaczenia wyswietlacza LCD do portow mikrokontrolera.
//---------------------------------------------------------------------------

static volatile uint8_t* port; // port[ 0 ] = PORTx, port[ -1 ] = DDRx 

static void hd44780_configure()
{
  port[  0 ] &= ~0b11111100; 
  port[ -1 ] |=  0b11111100;
}

inline void hd44780_rs_high() { port[ 0 ] |=  0b00000100; }
inline void hd44780_rs_low () { port[ 0 ] &= ~0b00000100; }
inline void hd44780_e_high () { port[ 0 ] |=  0b00001000; }
inline void hd44780_e_low  () { port[ 0 ] &= ~0b00001000; }

static void hd44780_write_half( uint8_t value )
{
  value <<= 4;
  value |= port[ 0 ] & 0x0f;
  port[ 0 ] = value;
  hd44780_e_high();
  hd44780_e_low();
}

//---------------------------------------------------------------------------
// Koniec opisu podlaczenia wyswietlacza LCD do portow mikrokontrolera.
//---------------------------------------------------------------------------

static void hd44780_write( uint8_t value )
{
  hd44780_write_half( value >> 4 );
  hd44780_write_half( value );
  _delay_us( 50 );
}

static void hd44780_write_command( uint8_t v ) { hd44780_rs_low (); hd44780_write( v ); }
static void hd44780_write_data   ( uint8_t v ) { hd44780_rs_high(); hd44780_write( v ); }

static void hd44780_set_daddr( uint8_t x ) { hd44780_write_command( 0b10000000 | ( x & 0b01111111 ) ); }

static char buf[ 32 ];
static uint8_t idx;

static void hd44780_update( char c )
{
  uint8_t y = idx & 0b10000000 ? 0x10 : 0x00;
  uint8_t x = idx & 0b01111111;
  if ( buf[ y + x ] != c )
  {
    buf[ y + x ] = c;
	y <<= 2;
	hd44780_set_daddr( y + x );
	hd44780_write_data( c );
  }
  ++idx;
}

static int hd44780_putchar( char c, FILE* )
{
  if ( c == '\a' )
  {
    idx = 0;
  }
  else if ( c == '\n' )
  {
    for ( ; ( idx & 0x7f ) < 16; )
	{
	  hd44780_update( ' ' );
	}
	idx ^= 0b10000000;
	idx &= 0b10000000;
  }
  else if ( ( idx & 0x7f ) < 16 )
  {
    hd44780_update( c );
  }
  return 0;
}

static FILE hd44780_stream;

void hd44780( FILE*& f, volatile uint8_t& r )
{
  port = &r;
  hd44780_configure();
  _delay_ms( 15 );
  for( uint8_t i = 0; i < 3; i++ )
  {
    hd44780_write_half( 0x03 );
    _delay_ms( 5 );
  }
  hd44780_write_half( 0x02 );
  _delay_ms( 1 );
  hd44780_write( 0b00101000 ); // 4 bits, 2 lines, 5x8 fonts
  _delay_us( 50 );
  hd44780_write( 0b00001000 ); // Display off, cursor off, blinking off 
  _delay_us( 50 );
  hd44780_write( 0b00000001 ); // Clear display
  _delay_ms( 2 );
  hd44780_write( 0b00000110 ); // auto right, no shift
  _delay_us( 50 );
  hd44780_write( 0b00001100 ); // Display on, cursor off, blinking off
  
#if 1
  // Definicja dodatkowych znakow
  hd44780_write_command( 0x48 );
  // Litera l - kod \01
  hd44780_write_data(0b00001100);
  hd44780_write_data(0b00000100);
  hd44780_write_data(0b00000110);
  hd44780_write_data(0b00000100);
  hd44780_write_data(0b00001100);
  hd44780_write_data(0b00000100);
  hd44780_write_data(0b00001110);
  hd44780_write_data(0b00000000);
  // Litera a - kod \02
  hd44780_write_data(0b00000000);
  hd44780_write_data(0b00000000);
  hd44780_write_data(0b00001110);
  hd44780_write_data(0b00000001);
  hd44780_write_data(0b00001111);
  hd44780_write_data(0b00010001);
  hd44780_write_data(0b00001111);
  hd44780_write_data(0b00000010);
#endif
  hd44780_stream.put = hd44780_putchar;
  hd44780_stream.flags = _FDEV_SETUP_WRITE;
  f = &hd44780_stream;
}

static int usart_putchar( char c, FILE* )
{
  if ( c == '\n' )
  {
    usart_putchar( '\r', NULL );
  }
  while ( ( UCSRA & 1 << UDRE ) == 0 );
  UDR = c;
  return 0;
}

static volatile char rxtab[ 64 ];
static volatile uint8_t rx_src, rx_dst;

uint8_t usart_rx_empty()
{
  return rx_src == rx_dst ? 1 : 0;
}

ISR( USART_RXC_vect )
{
  uint8_t next = ( rx_dst + 1 ) & 0b00111111;
  if ( next != rx_src )
  {
    rxtab[ rx_dst ] = UDR;
    rx_dst = next;
  }
}

static int usart_getchar( FILE* )
{
  while ( rx_src == rx_dst );
  uint8_t value = rxtab[ rx_src ];
  rx_src = ( rx_src + 1 ) & 0b00111111;
  return value;
}

static FILE usart_stream;
static FILE usart_stdin;

void usart( FILE*& f )
{
  // UART_SPEED bps, 8N1
  UBRRH = ( F_CPU / 8 / UART_SPEED - 1 ) / 256;
  UBRRL = ( F_CPU / 8 / UART_SPEED - 1 ) % 256;
  UCSRA |= 1 << U2X;
  UCSRB = 1 << TXEN | 1 << RXEN | 1 << RXCIE;
  usart_stream.put = usart_putchar;
  usart_stream.flags = _FDEV_SETUP_WRITE;
  f = &usart_stream;
  usart_stdin.get = usart_getchar;
  usart_stdin.flags = _FDEV_SETUP_READ;
  stdin = &usart_stdin;
  sei();
}
