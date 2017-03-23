/*

  main.c 
  
  Simple Menu for ATMEGA Controller

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
  
  Sposob podlaczenia przyciskow:
    PA0 - N
    PA1 - S
    PA2 - T
    PA3 - W
  Sposob podlaczenia wyswietlacza:
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
  
  /* assign default color value */
  if ( u8g_GetMode(&u8g) == U8G_MODE_R3G3B2 ) 
    u8g_SetColorIndex(&u8g, 255);     /* white */
  else if ( u8g_GetMode(&u8g) == U8G_MODE_GRAY2BIT )
    u8g_SetColorIndex(&u8g, 3);         /* max intensity */
  else if ( u8g_GetMode(&u8g) == U8G_MODE_BW )
    u8g_SetColorIndex(&u8g, 1);         /* pixel on */
}

void sys_setup_keys(void)
{
  // rezystory podciagajace
  PORTA |= 1 << PA3 | 1 << PA2 | 1 << PA1 | 1 << PA0;
}

#define KEY_NONE 0
#define KEY_PREV 1
#define KEY_NEXT 2
#define KEY_SELECT 3
#define KEY_BACK 4

uint8_t sys_get_key(void)
{
  uint8_t result = KEY_NONE;
  if ( (PINA & (1<<PA0)) == 0 ) 
    result = KEY_PREV;
  if ( (PINA & (1<<PA1)) == 0 ) 
    result = KEY_NEXT;
  if ( (PINA & (1<<PA2)) == 0 ) 
    result |= KEY_SELECT;
  if ( (PINA & (1<<PA3)) == 0 ) 
    result |= KEY_BACK;
  return result;
}

uint8_t sys_key_first = KEY_NONE;
uint8_t sys_key_second = KEY_NONE;
uint8_t sys_key_code = KEY_NONE;

void sys_debounce_key(void) 
{
  sys_key_second = sys_key_first;
  sys_key_first = sys_get_key();
    
  if ( sys_key_second == sys_key_first )
    sys_key_code = sys_key_first;
  else
    sys_key_code = KEY_NONE;
}

#define MENU_ITEMS 4
char *menu_strings[MENU_ITEMS] = { "First Line", "Second Item", "3333333", "abcdefg" };

uint8_t menu_current = 0;
uint8_t menu_redraw_required = 0;
uint8_t last_key_code = KEY_NONE;

void draw_menu(void) 
{
  uint8_t i, h;
  u8g_uint_t w, d;

  u8g_SetFont(&u8g, u8g_font_5x7);
  u8g_SetFontRefHeightText(&u8g);
  u8g_SetFontPosTop(&u8g);
  
  h = u8g_GetFontAscent(&u8g)-u8g_GetFontDescent(&u8g);
  w = u8g_GetWidth(&u8g);
  for( i = 0; i < MENU_ITEMS; i++ ) 
  {
    d = (w-u8g_GetStrWidth(&u8g, menu_strings[i]))/2;
    u8g_SetDefaultForegroundColor(&u8g);
    if ( i == menu_current ) 
    {
      u8g_DrawBox(&u8g, 0, i*h+1, w, h);
      u8g_SetDefaultBackgroundColor(&u8g);
    }
    u8g_DrawStr(&u8g, d, i*h, menu_strings[i]);
  }
}

void update_menu(void) 
{
  if ( sys_key_code != KEY_NONE && last_key_code == sys_key_code ) 
  {
    return;
  }
  last_key_code = sys_key_code;
  
  switch ( sys_key_code ) 
  {
    case KEY_NEXT:
      menu_current++;
      if ( menu_current >= MENU_ITEMS )
        menu_current = 0;
      menu_redraw_required = 1;
      break;
    case KEY_PREV:
      if ( menu_current == 0 )
        menu_current = MENU_ITEMS;
      menu_current--;
      menu_redraw_required = 1;
      break;
  }
}

int main(void)
{
  sys_setup_keys();
  u8g_setup();

  menu_redraw_required = 1;
  for(;;)
  {  
    
    sys_debounce_key();
    
    if (  menu_redraw_required != 0 ) 
    {
      u8g_FirstPage(&u8g);
      do
      {
        draw_menu();
      } while ( u8g_NextPage(&u8g) );
      menu_redraw_required = 0;
    }

    update_menu();
  }
}
