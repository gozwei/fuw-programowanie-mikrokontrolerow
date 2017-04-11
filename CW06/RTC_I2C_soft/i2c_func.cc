#include <avr/io.h>
#include <util/delay.h>
#include "i2c.h"

// Programowa obsluga transmisji I2C w trybie "master".
//
// Autor: Pawel Klimczewski, 20 marca 2010.

#define I2C_SCL  PC0 // Przyporzadkowanie sygnalow
#define I2C_SDA  PC1

#define I2C_PORT PORTC
#define I2C_DDR  DDRC
#define I2C_PIN  PINC

static void i2c_scl_true()
{
  // Linia SCL podlaczona do +5V przez rezystor podciagajacy.
  //
  // Wazna kolejnosc instrukcji.
  // Najpierw ustawiamy linie jako wyjsciowa w stanie +5V co umozliwia
  // poplyniecie wiekszego pradu i szybkie narosniecie sygnalu na linii.
  I2C_PORT |= 1 << I2C_SCL;
  // Zmieniamy konfiguracje na sygnal wejsciowy podlaczony do +5V przez
  // rezystor podciagajacy. Dzieki temu urzadzenie "slave" podlaczone 
  // do magistrali ma mozliwosc zewrzec linie do masy i przeslac informacje.
  I2C_DDR &= ~( 1 << I2C_SCL );
  // Komunikacja na magistrali I2C nie moze byc zbyt szybka (maksymalnie
  // 100 kHz).
  _delay_us( 10 );
}

static void i2c_scl_false()
{
  // Linia SCL zwarta do masy.
  I2C_DDR  |=    1 << I2C_SCL  ;
  I2C_PORT &= ~( 1 << I2C_SCL );
  _delay_us( 10 );
}

static void i2c_sda_true()
{
  // Linia SDA podlaczona do +5V przez rezystor podciagajacy.
  I2C_PORT |=    1 << I2C_SDA  ;
  I2C_DDR  &= ~( 1 << I2C_SDA );
}

static void i2c_sda_false()
{
  // Linia SDA zwarta do masy.
  I2C_DDR  |=    1 << I2C_SDA  ;
  I2C_PORT &= ~( 1 << I2C_SDA );
}

// Funkcja ustawia linie SDA w stan value, generujac przy tym impuls
// zegarowy na linii SCL.
// Jako rezultat przekazuje ostateczny (byc moze wymuszony przez uklad "slave")
// stan linii SDA.
inline bool i2c_sda( bool value = true )
{
  i2c_scl_false();
  if ( value )
    i2c_sda_true();
  else
    i2c_sda_false();
  i2c_scl_true();
  // Odczytanie stanu linii SDA. Ma sens tylko dla wartosci argumentu value
  // rownej true. W przeciwnym przypadku odczytana wartoscia bedzie zawsze 
  // false.
  return I2C_PIN & 1 << I2C_SDA;
}

void i2c_start()
{
  i2c_sda_false();
}

void i2c_stop()
{
  i2c_sda( false );
  i2c_sda_true();
}

void i2c_write( uint8_t x )
{
  for ( uint8_t mask = 0x80; mask; mask >>= 1 )
  {
    i2c_sda( x & mask );
  }
  // Nie analizujemy bitu potwierdzenia przekazanego przez uklad "slave".
  i2c_sda();
}

uint8_t i2c_read( uint8_t ack )
{
  uint8_t ret_val = 0;
  for ( uint8_t i = 8; i; --i )
  {
    ret_val <<= 1;
    ret_val |= i2c_sda();
  }
  i2c_sda( !ack );
  return ret_val;
}

void i2c_init()
{
}
