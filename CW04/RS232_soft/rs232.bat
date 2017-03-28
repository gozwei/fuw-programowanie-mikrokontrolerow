avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c rs232.cc
avr-g++ -mmcu=atmega32 -o rs232 rs232.o
avr-objcopy -O ihex rs232 rs232.hex
avr-size -C --mcu=atmega32 rs232
kamprogavrc -d -f rs232.hex