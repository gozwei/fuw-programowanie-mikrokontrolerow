avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c eeprom.cc
avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c biblio.cc
avr-g++ -mmcu=atmega32 -Wl,-u,vfprintf -lprintf_flt -lm -o eeprom eeprom.o biblio.o
avr-objcopy -O ihex eeprom eeprom.hex
avr-size -C --mcu=atmega32 eeprom
kamprogavrc -d -f eeprom.hex