#ifndef _BIBLIO_H
#define _BIBLIO_H

#include <stdio.h>

// Implementacja standardowego strumienia wyjsciowego stdout na wyswietlaczu
// alfanumerycznym LCD, 2 wiersze po 16 znakow, zgodnym z HD44780.
//
// Sposob podlaczenia wyswietlacza LCD.
//
// RW = GND
// RS = PX2
// E  = PX3
// D4 = PX4
// D5 = PX5
// D6 = PX6
// D7 = PX7
//
// Implementacja standardowego strumienia diagnostycznego stderr do portu
// RS232, UART_SPEED bps, 8N1.
//
// Sposob podlaczenia lacza RS232.
//
// TXD = PD1
//
// Autor: Pawel Klimczewski, 4 marca 2009.

// Konfiguracja wyswietlacza LCD.
void hd44780( FILE*& f, volatile uint8_t& r );

// Konfiguracja lacza szeregowego.
void usart( FILE*& f ); 

uint8_t usart_rx_empty();

#endif // _BIBLIO_H
