avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c nuty.cc
avr-g++ -mmcu=atmega32 -o nuty nuty.o
avr-objcopy -O ihex nuty nuty.hex
avr-size -C --mcu=atmega32 nuty
kamprogavrc -d -f nuty.hex