#ifndef _HD44780_H
#define _HD44780_H

// Zbior funkcji do obslugi alfanumerycznego wyswietlacza LCD opartego
// na sterowniku HD44780.
//
// Autor: Pawel Klimczewski, 1 marca 2009.

#include <inttypes.h>

void hd44780_write_command( uint8_t value );
void hd44780_write_data   ( uint8_t value );
uint8_t hd44780_read_command();
uint8_t hd44780_read_data   ();
void hd44780_write_text( const char* p );
void hd44780_goto( uint8_t, uint8_t );
void hd44780_clear_display();
void hd44780_return_home();
void hd44780_auto_left    ();
void hd44780_auto_right   ();
void hd44780_auto_scroll  ();
void hd44780_auto_noscroll();
void hd44780_display_on  ();
void hd44780_display_off ();
void hd44780_cursor_on   ();
void hd44780_cursor_off  ();
void hd44780_blinking_on ();
void hd44780_blinking_off();
void hd44780_shift_right ();
void hd44780_shift_left  ();
void hd44780_cursor_right();
void hd44780_cursor_left ();
void hd44780_set_caddr( uint8_t address );
void hd44780_set_daddr( uint8_t address );
void hd44780_initialize();

#endif // _HD44780_H
