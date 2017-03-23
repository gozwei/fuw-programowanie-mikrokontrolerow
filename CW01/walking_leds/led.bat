avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -c led.cc
avr-g++ -mmcu=atmega32 -o led led.o
avr-objcopy -O ihex led led.hex
avr-size -C --mcu=atmega32 led
kamprogavrc -d -f led.hex