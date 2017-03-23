# fuw-programowanie-mikrokontrolerow
Materiały dydaktyczne do Programowania Mikrokontrolerów AVR na wydziale Fizyki UW

## Zawartość poszczególnych ćwiczeń

### Ćwiczenia 1

### Ćwiczenia 2

Następujące zadania w ramach ćwiczeń:

1. **bomba** - rozbrajanie wirtualnej bomby za pomocą przycisków z sygnalizajcą stanu za pomocą pojedyńczej diody LED.
2. **delay1** - przykład zastosowania funkcji `_delay_ms()` do opóźniania wykonania programu.
3. **delay2** - przykład opóźniania ze zmiennym czasem. Warto porównać rozmiar programu ze stałym i zmiennym czasem opóźnienia.
4. **7seg_multiplex** - przykład wyświetlania stałej liczby na 4 wyświetlaczach 7-segmentowych.
5. **7seg_multiplex_count** - przykład licznika czasu zrealizowany na multipleksowanym, 4-cyfrowym wyświetlaczu 7-segmentowym.
6. **7seg_multiplex_count** - przykład licznika czasu zrealizowany na multipleksowanym, 4-cyfrowym wyświetlaczu 7-segmentowym (alternatywne rozwiązanie, autor: Damian Włodzyński).

### Ćwiczenia 3

Następujące zadania w ramach ćwiczeń:

1. **HD44780A** - wyświetlanie tekstu na wyświetlaczu alfanumerycznym. W przykładzie napis przechowywany jest w programie jako stała (kompiowana do RAM) i wysyłany do wyświetlaczas znak po znaku. Należy zwrócić uwagę, na przełączenie linii wyświetlacza do 40 znaków (bajtów).
2. **HD44780B** - przykład analogiczny do poprzedniego, wykorzytsujący funkcje `PSTR()` oraz `pgm_read_byte()`, dzięki którym napis nie musi być przechowywany w pamięci RAM. Należy porównać użycie pamięci obydwu programów za pomocą polecania `avr-size -C --mcu=atmega32 char_lcd`.
3. **HD44780C** - przykład użycia biblioteki `stdio` i funkcji `printf()` do umieszczania znaków na wyświetlaczu alfanumerycznym. Przykład zawiera formatowanie liczby zmiennoprzecinkowej, co wymaga dodania opcji `-Wl,-u,vfprintf -lprintf_flt -lm` do kroku linkowania. 
4. **graph** - zestaw przykładów wykorzystania wyświetlacza graficznego. Należy zwrócić uwagę na lokalizację biblioteki u8glib, ustawianą w pliku `Makefile`. Kompilacja przykładów wymaga wywołania `make` i `make upload` do wgrania programu do mikrokontrolera.  

## Źródła (folder `_inlcude`)

### `u8glib`

`u8glib_curr` - wersja pobrana z https://github.com/olikraus/u8glib/tree/master/csrc

`u8glib_oryg` - wersja oryginalnie dołączona do przykłaów