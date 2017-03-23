/*

  main.c 
  
  U8g Logo for ATMEGA Controller

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

u8g_t u8g;

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
}

void draw_color_box(void)
{
  u8g_uint_t w,h;
  u8g_uint_t r, g, b;
  
  w = u8g_GetWidth(&u8g)/32;
  h = u8g_GetHeight(&u8g)/8;
  for( b = 0; b < 4; b++ )
    for( g = 0; g < 8; g++ )
      for( r = 0; r < 8; r++ )
      {
        u8g_SetColorIndex(&u8g, (r<<5) |  (g<<2) | b );
        u8g_DrawBox(&u8g, g*w + b*w*8, r*h, w, h);
      }
}

void draw_logo(uint8_t d)
{
  u8g_SetFont(&u8g, u8g_font_gdr25r);
  u8g_DrawStr(&u8g, 0+d, 30+d, "U");
  u8g_SetFont(&u8g, u8g_font_gdr30n);
  u8g_DrawStr90(&u8g, 23+d,10+d,"8");
  u8g_SetFont(&u8g, u8g_font_gdr25r);
  u8g_DrawStr(&u8g, 53+d,30+d,"g");
  
  u8g_DrawHLine(&u8g, 2+d, 35+d, 47);
  u8g_DrawVLine(&u8g, 45+d, 32+d, 12);
}

void draw_url(void)
{
  u8g_SetFont(&u8g, u8g_font_4x6);
  if ( u8g_GetHeight(&u8g) < 59 )
  {
    u8g_DrawStr(&u8g, 53,9,"code.google.com");
    u8g_DrawStr(&u8g, 77,18,"/p/u8glib");
  }
  else
  {
    u8g_DrawStr(&u8g, 1,54,"code.google.com/p/u8glib");
  }
}


void draw(void) 
{
  if ( u8g_GetMode(&u8g) == U8G_MODE_R3G3B2 ) 
  {
    draw_color_box();
  }
  u8g_SetColorIndex(&u8g, 1);
  if ( U8G_MODE_GET_BITS_PER_PIXEL(u8g_GetMode(&u8g)) > 1 ) 
  {
    draw_logo(2);
    u8g_SetColorIndex(&u8g, 2);
    draw_logo(1);
    u8g_SetColorIndex(&u8g, 3);
  }
  draw_logo(0);
  draw_url();
}

int main(void)
{
  u8g_setup();
  
  for(;;)
  {  
    u8g_FirstPage(&u8g);
    do
    {
      draw();
    } while ( u8g_NextPage(&u8g) );
    u8g_Delay(10);
  }
}

