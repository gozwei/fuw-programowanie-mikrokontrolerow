avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c bomba.cc
avr-g++ -mmcu=atmega32 -o bomba bomba.o
avr-objcopy -O ihex bomba bomba.hex
avr-size -C --mcu=atmega32 bomba
kamprogavrc -d -f bomba.hex