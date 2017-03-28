avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c zegar.cc
avr-g++ -mmcu=atmega32 -o zegar zegar.o
avr-objcopy -O ihex zegar zegar.hex
avr-size -C --mcu=atmega32 zegar
kamprogavrc -d -f zegar.hex