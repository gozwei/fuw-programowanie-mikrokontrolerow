#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include "biblio.h"

// Wymagane polaczenia w ukladzie ZL15AVR:
//
//  TxD - PD1
//
// Autor: Pawel Klimczewski, 19 marca 2012.

int main()
{
  usart( stdout );
  printf( "Taki sobie licznik\n" );
  uint8_t count = 0; 
  while ( true )
  {
	count++;
    printf("%d\n", count);
    //_delay_ms(1000);
  }
  return 0;
}