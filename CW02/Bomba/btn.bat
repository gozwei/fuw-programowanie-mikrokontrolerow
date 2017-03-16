avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -c btn.cc
avr-g++ -mmcu=atmega32 -o btn btn.o
avr-objcopy -O ihex btn btm.hex