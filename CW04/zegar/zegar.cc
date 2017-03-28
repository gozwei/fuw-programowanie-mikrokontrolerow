// Program zegara. Mikrokontroler pracuje z czestotliwoscia 16 MHz.
//
// Wymagane polaczenia w ukladzie ZL15AVR:
//        PC0 - 7SEG A
//        PC1 - 7SEG B
//        ...
//        PC5 - 7SEG F
//        PC6 - 7SEG G
//        PC7 - 7SEG .
//        PA0 - 7SEG 0
//        PA1 - 7SEG 1
//        PA2 - 7SEG 2
//        PA3 - 7SEG 3
//
// Autor: Pawel Klimczewski, 12 marca 2012.

#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>

void configure_pins()
{
  DDRC = 0xff; // Wyprowadzenia PC7..PC0 konfiguruje jako wyjscia.
  DDRA = 0x0f; // Wyprowadzenia PA3..PA0 konfiguruje jako wyjscia.
}

void configure_timer()
{
  // Licznik 0 zlicza z czestotliwoscia F_CPU / 256 = 62 500 Hz
  // Przepelnienie nastepuje co 4 ms (244 Hz) i generuje przerwanie
  // TIMER0_OVF.
  TCCR0 = 1 << CS02;
  TIMSK |= 1 << TOIE0;
  
  // Licznik 1 zlicza z czestotliwoscia F_CPU / 256 = 62 500 Hz.
  // Zakres licznika wynosi 31 250.
  // Co 0.5 s generowane jest przerwanie TIMER1_COMPA.
  TCCR1B = 1 << CS12 | 1 << WGM12;
  OCR1A = 31250;
  TIMSK |= 1 << OCIE1A;
}

uint8_t digits[ 10 ] = { 63, 6, 91, 79, 102, 109, 125, 7, 127, 111 };
uint8_t seg[ 4 ] = { 1 << PA3, 1 << PA2, 1 << PA1, 1 << PA0 };
uint8_t buf[ 4 ] = { 0, 0, 0, 0 };

volatile uint8_t s = 0; // Licznik sekund.

ISR( TIMER1_COMPA_vect )
{
  // Tutaj jestesmy raz na sekunde.
  if ( ++s < 120 ) return;
  // Tutaj jestesmy raz na minute.
  s = 0;
  // Zwiekszamy cyfre jednostek minut.
  if ( ++buf[ 3 ] == 10 )
  {
    buf[ 3 ] = 0;
    // Zwiekszamy cyfre dziesiatek minut.
    if ( ++buf[ 2 ] == 6 )
    {
      buf[ 2 ] = 0;
      // Zwiekszamy cyfre jednostek godzin.
      if ( ++buf[ 1 ] == 10 )
      {
        buf[ 1 ] = 0;
        // Zwiekszamy cyfre dziesiatek godzin.
        ++buf[ 0 ];
      }
      // Sprawdzamy zakres liczby godzin.
      if ( buf[ 0 ] == 2 && buf[ 1 ] == 4 )
      {
        buf[ 0 ] = buf[ 1 ] = 0;
      }
    }
  }
}

volatile uint8_t i = 0; // Licznik aktualnie wlaczonej cyfry.

ISR( TIMER0_OVF_vect )
{
  // Funkcja jest wykonywana ok. 244 razy w ciagu sekundy.
  
  // Wylaczam wszystkie wyswietlacze.
  PORTA |= 1 << PA3 | 1 << PA2 | 1 << PA1 | 1 << PA0;
  // Wyswietlam i-ta cyfre
  PORTC = ~digits[ buf[ i ] ];
  // Wlaczam i-ty wyswietlacz.
  PORTA &= ~seg[ i ];
  // Na drugim wyswietlaczu zapalam kropke dziesietna.
  if ( i == 1 )
  {
    if ( s & 1 ) 
      PORTC |= 1 << PC7;
    else
      PORTC &= ~( 1 << PC7 );
  }
  ++i;
  if ( i == 4 )
    i = 0;  
}

int main()
{
  configure_pins();
  configure_timer();
  sei();
  while ( true );
  return 0;
}
