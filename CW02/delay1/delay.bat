avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c delay.cc
avr-g++ -mmcu=atmega32 -o delay delay.o
avr-objcopy -O ihex delay delay.hex
avr-size -C --mcu=atmega32 delay
kamprogavrc -d -f delay.hex