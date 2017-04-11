#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include "i2c.h"
#include "biblio.h"

// Obsluga ukladu zegara.
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
//  SCL (U6 RTC/con7) - PC0
//  SDA (U6 RTC/con7) - PC1
//
// Autor: Pawel Klimczewski, 20 marca 2010.

int main()
{
  hd44780( stdout, PORTC );
  i2c_init();
  while ( true )
  {
    i2c_start();                      
    i2c_write( 0xd0 ); // adres ukladu = 104, zapis                   
    i2c_write( 0x00 );    
    i2c_stop();
    
    i2c_start();                   
    i2c_write( 0xd1 ); // adres ukladu = 104, odczyt                
    uint8_t s = i2c_read( ACK  ) & 0x7f;       
    uint8_t m = i2c_read( ACK  ) & 0x7f;       
    uint8_t h = i2c_read( NACK ) & 0x3f;
    i2c_stop();
    
    printf( "%2x%c%02x\n\n", h, s & 0x01 ? ':' : ' ', m );    
  }
  return 0;
}
