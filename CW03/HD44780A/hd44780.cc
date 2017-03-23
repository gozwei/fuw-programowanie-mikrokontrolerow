#include <avr/io.h>
#include <util/delay.h>
#include "hd44780.h"

// Zbior funkcji do obslugi alfanumerycznego wyswietlacza LCD opartego
// na sterowniku HD44780.
//
// Autor: Pawel Klimczewski, 1 marca 2009.

// Linie sterujace.
//
// PC0 = RS
// PC1 = RW
// PC2 = E
//
// Linie zwiazane z przesylaniem danych.
//
// PD0 = D0
// PD1 = D1
// PD2 = D2
// PD3 = D3
// PD4 = D4
// PD5 = D5
// PD6 = D6
// PD7 = D7
//
// Zdefiniowanie stalej DL8 odpowiada transmisji 8 bitowej,
// a pominiecie definicji - transmisji 4 bitowej.

#define DL8

//---------------------------------------------------------------------------
// Opis podlaczenia wyswietlacza do portow mikrokontrolera.
//---------------------------------------------------------------------------

static void hd44780_configure_ctrl_lines()
{
  DDRC  |=    1 << PC0 | 1 << PC1 | 1 << PC2  ;
  PORTC &= ~( 1 << PC0 | 1 << PC1 | 1 << PC2 );
}

inline void hd44780_rs_high() { PORTC |=    1 << PC0  ; }
inline void hd44780_rs_low () { PORTC &= ~( 1 << PC0 ); }
inline void hd44780_rw_high() { PORTC |=    1 << PC1  ; }
inline void hd44780_rw_low () { PORTC &= ~( 1 << PC1 ); }
inline void hd44780_e_high () { PORTC |=    1 << PC2  ; }
inline void hd44780_e_low  () { PORTC &= ~( 1 << PC2 ); }

static void hd44780_configure_data_lines_as_outputs()
{
#ifdef DL8
  DDRD = 0xff;
#else // DL8
  DDRD |= 0xf0;
#endif // DL8
}

static void hd44780_configure_data_lines_as_inputs()
{
#ifdef DL8
  DDRD = PORTD = 0;
#else // DL8
  DDRD  &= 0x0f;
  PORTD &= 0x0f;
#endif // DL8
}

#ifndef DL8
static uint8_t hd44780_read_half()
{
  hd44780_e_high();
  _delay_us( 1 );
  uint8_t value = PIND;
  hd44780_e_low();
  return value >> 4;
}
#endif // DL8

static uint8_t hd44780_read()
{
  hd44780_configure_data_lines_as_inputs();
  hd44780_rw_high();
#ifdef DL8
  hd44780_e_high();
  _delay_us( 1 );
  uint8_t value = PIND;
  hd44780_e_low();
#else // DL8
  uint8_t value = hd44780_read_half() << 4;
  value |= hd44780_read_half();
#endif // DL8
  hd44780_rw_low();
  hd44780_configure_data_lines_as_outputs();
  return value;
}

static void hd44780_write_half( uint8_t value )
{
  value <<= 4;
  value |= PORTD & 0x0f;
  PORTD = value;
  hd44780_e_high();
  hd44780_e_low();
}

static void hd44780_write( uint8_t value )
{
#ifdef DL8
  PORTD = value;
  hd44780_e_high();
  hd44780_e_low();
#else // DL8
  hd44780_write_half( value >> 4 );
  hd44780_write_half( value );
#endif // DL8
  _delay_us( 50 );
}

//---------------------------------------------------------------------------
// Koniec opisu podlaczenia wyswietlacza do portow mikrokontrolera.
//---------------------------------------------------------------------------

uint8_t hd44780_read_command() { hd44780_rs_low (); return hd44780_read(); }
uint8_t hd44780_read_data   () { hd44780_rs_high(); return hd44780_read(); }

static void hd44780_loop_until_ready()
{
  //while ( hd44780_read_command() & 0b10000000 );
}

static void hd44780_loop_until_ready_and_write( uint8_t value )
{
  hd44780_loop_until_ready();
  hd44780_write( value );
}

void hd44780_write_command( uint8_t v ) { hd44780_rs_low (); hd44780_loop_until_ready_and_write( v ); }
void hd44780_write_data   ( uint8_t v ) { hd44780_rs_high(); hd44780_loop_until_ready_and_write( v ); }

void hd44780_write_text( const char* p )
{
  while ( *p )
    hd44780_write_data( *p++ );
}

void hd44780_clear_display()
{
  hd44780_write_command( 0b00000001 );
  _delay_ms( 2 );
}

void hd44780_return_home()
{
  hd44780_write_command( 0b00000010 );
  _delay_ms( 2 );
}

static uint8_t cfg1 = 0b00000110;

void hd44780_auto_right   () { cfg1 |=  0b00000010; hd44780_write_command( cfg1 ); }
void hd44780_auto_left    () { cfg1 &= ~0b00000010; hd44780_write_command( cfg1 ); }
void hd44780_auto_scroll  () { cfg1 |=  0b00000001; hd44780_write_command( cfg1 ); }
void hd44780_auto_noscroll() { cfg1 &= ~0b00000001; hd44780_write_command( cfg1 ); }

static uint8_t cfg2 = 0b00001000;

void hd44780_display_on  () { cfg2 |=  0b00000100; hd44780_write_command( cfg2 ); }
void hd44780_display_off () { cfg2 &= ~0b00000100; hd44780_write_command( cfg2 ); }
void hd44780_cursor_on   () { cfg2 |=  0b00000010; hd44780_write_command( cfg2 ); }
void hd44780_cursor_off  () { cfg2 &= ~0b00000010; hd44780_write_command( cfg2 ); }
void hd44780_blinking_on () { cfg2 |=  0b00000001; hd44780_write_command( cfg2 ); }
void hd44780_blinking_off() { cfg2 &= ~0b00000001; hd44780_write_command( cfg2 ); }

void hd44780_shift_right () { hd44780_write_command( 0b00011100 ); }
void hd44780_shift_left  () { hd44780_write_command( 0b00011000 ); }
void hd44780_cursor_right() { hd44780_write_command( 0b00010100 ); }
void hd44780_cursor_left () { hd44780_write_command( 0b00010000 ); }

void hd44780_set_caddr( uint8_t x ) { hd44780_write_command( 0b01000000 | ( x & 0b00111111 ) ); }

void hd44780_set_daddr( uint8_t x ) { hd44780_write_command( 0b10000000 | ( x & 0b01111111 ) ); }

void hd44780_goto( uint8_t x, uint8_t y )
{
  hd44780_set_daddr( x + 0x40 * y );
}

void hd44780_initialize()
{
  hd44780_configure_ctrl_lines();
  hd44780_configure_data_lines_as_outputs();
  _delay_ms( 15 );
  for( uint8_t i = 0; i < 3; i++ )
  {
    hd44780_write_half( 0x03 );
    _delay_ms( 5 );
  }
#ifdef DL8
  hd44780_write( 0b00111000 ); // 4 bits, 2 lines, 5x8 fonts
  _delay_us( 50 );
#else // DL8
  hd44780_write_half( 0x02 );
  _delay_ms( 1 );
  hd44780_write( 0b00101000 ); // 4 bits, 2 lines, 5x8 fonts
  _delay_us( 50 );
#endif // DL8
  hd44780_write( cfg2 );
  _delay_us( 50 );
  hd44780_write( 0b00000001 ); // Clear display
  _delay_ms( 2 );
  hd44780_write( cfg1 );
  _delay_us( 50 );
  hd44780_display_on();
}
