#include <avr/io.h>
#include <avr/eeprom.h>
#include <inttypes.h>
#include "biblio.h"

// Obsluga pamieci EEPROM.
//
// Wymagane polaczenia w ukladzie ZL15AVR:
//
//  LCD TEXT RW - GND
//  LCD TEXT RS - PC2
//  LCD TEXT E  - PC3
//  LCD TEXT D4 - PC4
//  LCD TEXT D5 - PC5
//  LCD TEXT D6 - PC6
//  LCD TEXT D7 - PC7
//
// Autor: Pawel Klimczewski, 22 marca 2010.

int main()
{
  hd44780( stdout, PORTC );
  uint8_t i;
  // Odczytuje bajt pamieci o adresie 0.
  i = eeprom_read_byte( 0 );
  printf( "eeprom[0]=%d\n\n", i );
  ++i;
  // Zapisuje bajt pamieci o adresie 0.
  eeprom_write_byte( 0, i );
  return 0;
}
