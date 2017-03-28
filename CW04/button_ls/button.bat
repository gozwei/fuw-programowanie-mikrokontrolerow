avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c button.cc
avr-g++ -mmcu=atmega32 -o button button.o
avr-objcopy -O ihex button button.hex
avr-size -C --mcu=atmega32 button
kamprogavrc -d -f button.hex