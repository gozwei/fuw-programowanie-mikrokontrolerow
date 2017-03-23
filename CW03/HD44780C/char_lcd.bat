avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c char_lcd.cc
avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c hd44780.cc
avr-g++ -mmcu=atmega32 -Wl,-u,vfprintf -lprintf_flt -lm -o char_lcd char_lcd.o hd44780.o
avr-objcopy -O ihex char_lcd char_lcd.hex
avr-size -C --mcu=atmega32 char_lcd
kamprogavrc -d -f char_lcd.hex