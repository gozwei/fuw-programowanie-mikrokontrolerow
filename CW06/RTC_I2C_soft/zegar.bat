avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c zegar.cc
avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c biblio.cc
avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c i2c_func.cc
avr-g++ -mmcu=atmega32 -Wl,-u,vfprintf -lprintf_flt -lm -o zegar zegar.o biblio.o i2c_hw.o
avr-objcopy -O ihex zegar zegar.hex
avr-size -C --mcu=atmega32 zegar
kamprogavrc -d -f zegar.hex