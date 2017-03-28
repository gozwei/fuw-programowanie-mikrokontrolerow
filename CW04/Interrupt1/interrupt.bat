avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c interrupt.cc
avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c biblio.cc
avr-g++ -mmcu=atmega32 -o interrupt interrupt.o biblio.o
avr-objcopy -O ihex interrupt interrupt.hex
avr-size -C --mcu=atmega32 interrupt
kamprogavrc -d -f interrupt.hex