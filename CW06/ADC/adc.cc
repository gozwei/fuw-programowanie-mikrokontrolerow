#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include "biblio.h"

// Obsluga przetwornika analogowo-cyfrowego.
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
//  P2 (con7) - PA0
//
// Autor: Pawel Klimczewski, 20 marca 2010.

uint16_t adc( uint8_t channel = 0 )
{
  ADMUX = 
    1 << REFS0 |                          // Napiecie odniesienia = VCC 
    ( channel & 0x7 );
  ADCSRA = 
    1 << ADEN |                           // Przetwornik wlaczony
    1 << ADSC |                           // Rozpoczecie konwersji
    1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0; // Predkosc pracy F_CPU / 128.
  loop_until_bit_is_clear( ADCSRA, ADSC );
  uint16_t value = ADCL;
  value |= ADCH << 8;
  return value;
}

int main()
{
  //hd44780( stdout, PORTC );
  usart(stdout);
  while ( true )
  {
    printf( "%u\n", adc() );
	_delay_ms(150);
  }
  return 0;
}
