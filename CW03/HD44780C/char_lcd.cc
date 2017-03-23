#include <stdio.h>
#include <util/delay.h>
#include "hd44780.h"

// Powiazanie strumienia wyjsciowego stdout z wyswietlaczem LCD.
//
// Autor: Pawel Klimczewski, 2 marca 2009.

int hd44780_putchar( char c, FILE* )
{
  hd44780_write_data( c );
  return 0;
}

FILE hd44780_stdout;

int main()
{
  hd44780_initialize();
  hd44780_cursor_on();
  hd44780_blinking_on();
  hd44780_stdout.put = hd44780_putchar;
  hd44780_stdout.flags = _FDEV_SETUP_WRITE;
  stdout = &hd44780_stdout;
  printf( "Hello world %03d!",15);
  hd44780_goto(0,1);
  printf( "%10.4f",1.0/17.0 );
  return 0;
}