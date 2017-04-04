avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c tc77_hard.cc
avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c biblio.cc
avr-g++ -mmcu=atmega32 -Wl,-u,vfprintf -lprintf_flt -lm -o tc77_hard tc77_hard.o biblio.o
avr-objcopy -O ihex tc77_hard tc77_hard.hex
avr-size -C --mcu=atmega32 tc77_hard
kamprogavrc -d -f tc77_hard.hex