avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c sleep.cc
avr-g++ -mmcu=atmega32 -Wl,-u,vfprintf -lprintf_flt -lm -o sleep sleep.o biblio.o i2c_hw.o
avr-objcopy -O ihex sleep sleep.hex
avr-size -C --mcu=atmega32 sleep
kamprogavrc -d -f sleep.hex