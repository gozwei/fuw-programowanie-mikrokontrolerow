/*

  main.c 
  
  Hello World for ATMEGA Controller

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

void draw(void)
{
  u8g_SetFont(&u8g, u8g_font_6x10);
  u8g_DrawStr(&u8g, 0, 15, "Hello World!");
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
    u8g_Delay(100);
  }
  
}
