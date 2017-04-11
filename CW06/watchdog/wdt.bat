avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c wdt.cc
avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c biblio.cc
avr-g++ -mmcu=atmega32 -Wl,-u,vfprintf -lprintf_flt -lm -o wdt wdt.o biblio.o
avr-objcopy -O ihex wdt wdt.hex
avr-size -C --mcu=atmega32 wdt
kamprogavrc -d -f wdt.hex