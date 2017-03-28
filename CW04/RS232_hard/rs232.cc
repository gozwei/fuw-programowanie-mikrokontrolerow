#include <avr/io.h>
#include <util/delay.h>

// RS232 - hello world.
//
// Autor: Pawel Klimczewski, 2 marca 2009.

void configure_usart()
{
  // 9600 bps, 8N1
  UBRRH = ( F_CPU / 16 / 9600 - 1 ) / 256;
  UBRRL = ( F_CPU / 16 / 9600 - 1 ) % 256;
  UCSRB = 1 << TXEN;
}

int main()
{
  configure_usart();
  while ( true )
  {
    const char* p = "Hello world!!!\r\n";
    for ( ; *p; ++p )
    {
      UDR = *p;
      for ( uint8_t i = 15; i; --i )
        _delay_ms( 10 );
    }
  }
  return 0;
}
