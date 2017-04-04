/*

  main.c 
  
  Graphics Test for ATMEGA Controller

  Universal 8bit Graphics Library
  
  Copyright (c) 2012, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  
  
*/

/*
  
  Przyklad dostosowany do zestawu ZL15AVR.
  
  Sposob podlaczenia wyswietlacza.
    PC0 - E
    PC1 - CS1
    PC2 - CS2
    PC3 - RS
    PC4 - RW
    PD0 - D0
    PD1 - D1
    ...
    PD6 - D6
    PD7 - D7
    
  Pawel Klimczewski, 7 marca 2014
    
*/

#include <avr/interrupt.h>
#include <avr/io.h>
#include "u8g.h"
#include <inttypes.h>
#include <util/delay.h>
//#include "biblio.h"
u8g_t u8g;

static volatile char mystr[16];

static volatile char mystr2[16];
static volatile char mystr3[16];
static volatile uint32_t values[128];
static volatile	float min = 22.;
static volatile	float max = 23.;
void u8g_setup(void)
{  
  u8g_Init8Bit(&u8g, &u8g_dev_ks0108_128x64,
               // Linie danych podlaczone do portu D
               PN(3, 0), // D0  - PD0
               PN(3, 1), // D1  - PD1
               PN(3, 2), // D2  - PD2
               PN(3, 3), // D3  - PD3
               PN(3, 4), // D4  - PD4 
               PN(3, 5), // D5  - PD5 
               PN(3, 6), // D6  - PD6 
               PN(3, 7), // D7  - PD7
               // Linie sterujace podlaczone do portu C
               PN(2, 0), // E   - PC0 
               PN(2, 1), // CS1 - PC1 
               PN(2, 2), // CS2 - PC2 
               PN(2, 3), // RS  - PC3 
               PN(2, 4), // RW  - PC4 
               U8G_PIN_NONE);
  
  /* assign default color value */
  if ( u8g_GetMode(&u8g) == U8G_MODE_R3G3B2 ) 
    u8g_SetColorIndex(&u8g, 255);     /* white */
  else if ( u8g_GetMode(&u8g) == U8G_MODE_GRAY2BIT )
    u8g_SetColorIndex(&u8g, 3);         /* max intensity */
  else if ( u8g_GetMode(&u8g) == U8G_MODE_BW )
    u8g_SetColorIndex(&u8g, 1);         /* pixel on */
}

void u8g_prepare(void) {
  u8g_SetFont(&u8g, u8g_font_6x10);
  u8g_SetFontRefHeightExtendedText(&u8g);
  u8g_SetDefaultForegroundColor(&u8g);
  u8g_SetFontPosTop(&u8g);
}


void draw(void) {
  u8g_prepare();
  u8g_DrawStr(&u8g, 0,01, mystr);
  u8g_DrawStr(&u8g, 110,0, mystr2);
  u8g_DrawStr(&u8g, 110,55, mystr3);
  uint32_t val;

  for(int a =0; a<128;a++)
  {
	val = values[a];
	u8g_DrawPixel(&u8g, a, (int)(64-(((val / 16.)-min)*(1)/(max-min))*64.));
  }
}

void spi_init()
{
  DDRB |= 1 << PB7 | 1 << PB4;
  // Uklad odbiornika SPI pracuje w trybie "master". Czestotliwosc zegara
  // F_CPU/16.
  SPCR = 1 << SPE | 1 << MSTR | 1 << SPR0;
}

uint8_t spi_transmit( uint8_t v )
{
  SPDR = v;
  while ( !( SPSR & 1 << SPIF ) );
  return SPDR;
}


// Koniec opisu podlaczenia ukladu tc77.

uint16_t read16()
{
  uint16_t value = spi_transmit( 0 );
  value <<= 8;
  value |= spi_transmit( 0 );
  return value;
}

int main(void)
{
  u8g_setup();
  u8g_prepare();
	spi_init();
	uint8_t idx = 0;
  for(;;)
  {  
	// Sygnalizacja rozpoczecia transmisji.
    PORTB &= ~( 1 << PB4 );
    // Odczytanie temperatury (bity 15-3) i bitu kontrolnego (2).
    uint16_t value = read16();
    // Sygnalizacja zakonczenia transmisji.
    PORTB |=    1 << PB4  ;
    
    if ( value & 0x0004 )
    {
      value >>= 3;
      // Formatowanie zmiennopozycyjne wymaga dodatkowych opcji konsolidacji
      //   -Wl,-u,vfprintf -lprintf_flt -lm
      // i znaczaco zwieksza rozmiar programu.
      sprintf(mystr, "%.1f\n", value / 16. );
	  sprintf(mystr2, "%.2f\n", max );
	  sprintf(mystr3, "%.2f\n", min );
		values[idx] = value;
	
	uint16_t lmin  = 100;
	uint16_t lmax = -100;
	for(int a = 0; a<128; a++)
	{
		if(lmin > values[a])
			lmin = values[a];
		if(lmax < values[a])
			lmax = values[a];
	}
	max = 32;//lmax / 16.;
	min = 26;//lmin / 16.;
	
	
		idx++;
		if(idx==128)
			idx = 0;
    }
    else
    {
      //printf( "czekaj\n" );
	  
    }
	
	u8g_FirstPage(&u8g);
    do
    {
      draw();
    } while ( u8g_NextPage(&u8g) );
    
    _delay_ms( 50 );
	
  }
}
