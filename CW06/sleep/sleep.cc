#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

// Program demonstruje przelaczenie mikrokontrolera do trybu uspienia.
//
// Sposob podlaczenia w ukladzie ZL15AVR.
//
// PA0 - LED0
// PD2 (INT0) - SW0
// PD3 (INT1) - SW1
//
// Autor: Pawel Klimczewski, 19 marca 2012.

//---------------------------------------------------------------------------
void configure_pins()
{
  DDRA |= 1 << PA0; // Port PA0 steruje dioda LED.
}
//---------------------------------------------------------------------------
void blink_1()
{
  PORTA |= 1 << PA0;
  _delay_ms( 250 );
  PORTA &= ~( 1 << PA0 );
  _delay_ms( 250 );
}
//---------------------------------------------------------------------------
void blink_n( uint8_t n )
{
  for ( uint8_t i = n; i; --i )
  {
    blink_1();
  }
}
//---------------------------------------------------------------------------
inline void int0_int1_enable()
{
  // Wlaczam obsluge przerwan INT0 i INT1. Niski poziom sygnalu na wejsciu
  // spowoduje wybudzenie mikrokontrolera i wygeneruje przerwanie.
  GICR |= 1 << INT1 | 1 << INT0;  
}
//---------------------------------------------------------------------------
inline void int0_int1_disable()
{
  // Wylaczam obsluge przerwan INT0 i INT1.
  GICR &= ~( 1 << INT1 | 1 << INT0 );  
}
//---------------------------------------------------------------------------
void sleep()
{
  // Wylaczam rezystory podciagajace dla wejsc PD2 i PD3 aby zmniejszyc 
  // zuzycie energii w trybie uspienia.
  PORTD &= ~( 1 << PD2 | 1 << PD3 );
  set_sleep_mode( SLEEP_MODE_PWR_DOWN );
  cli();
  sleep_enable();
  sei();
  sleep_cpu();
  sleep_disable();
  sei();
}
//---------------------------------------------------------------------------
uint8_t n;

ISR( INT0_vect )
{
  n = 3;
  int0_int1_disable();
}
//---------------------------------------------------------------------------
ISR( INT1_vect )
{
  n = 5;
  int0_int1_disable();
}
//---------------------------------------------------------------------------
int main()
{
  configure_pins();
  n = 2;
  while ( true )
  {
    blink_n( n );
    // Rezystory podciagajace dla PD2 i PD3.
    PORTD |= 1 << PD2 | 1 << PD3; 
    // Czekam, az zaden przycisk nie jest nacisniety.
    while ( ( PIND & ( 1 << PD2 | 1 << PD3 ) ) != 
            ( 1 << PD2 | 1 << PD3 ) );
    int0_int1_enable();
    sleep();
  }
  return 0;
}
//---------------------------------------------------------------------------
