#include <avr/io.h>
#include "i2c.h"

// Sprzetowa obsluga transmisji I2C.
//
// Autor: Pawel Klimczewski, 20 marca 2010.

void i2c_start()
{
  TWCR = 1 << TWINT | 1 << TWEN | 1 << TWSTA;
  loop_until_bit_is_set( TWCR, TWINT );
}

void i2c_stop()
{
  TWCR = 1 << TWINT | 1 << TWEN | 1 << TWSTO;
  loop_until_bit_is_clear( TWCR, TWSTO );
}

void i2c_write( uint8_t data )
{
  TWDR = data;
  TWCR = 1 << TWINT | 1 << TWEN;
  loop_until_bit_is_set( TWCR, TWINT );
}

uint8_t i2c_read( uint8_t ack )
{
  TWCR = ack 
       ? 1 << TWINT | 1 << TWEN | 1 << TWEA 
       : 1 << TWINT | 1 << TWEN;
  loop_until_bit_is_set( TWCR, TWINT );
  return TWDR;
}

void i2c_init()
{
  // Czestotliwosc sygnalu SCL = F_CPU / ( 16 + 2 * TWBR ) = 50 kHz. 
  TWBR = 152;
}
