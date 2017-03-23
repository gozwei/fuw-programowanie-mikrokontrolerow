avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c 7seg.cc
avr-g++ -mmcu=atmega32 -o 7seg 7seg.o
avr-objcopy -O ihex 7seg 7seg.hex
avr-size -C --mcu=atmega32 7seg
kamprogavrc -d -f 7seg.hex