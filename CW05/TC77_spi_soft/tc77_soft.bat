avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c tc77_soft.cc
avr-g++ -mmcu=atmega32 -DF_CPU=16000000 -Wall -O2 -gstabs -ffunction-sections -fdata-sections -c biblio.cc
avr-g++ -mmcu=atmega32 -o tc77_soft tc77_soft.o biblio.o
avr-objcopy -O ihex tc77_soft tc77_soft.hex
avr-size -C --mcu=atmega32 tc77_soft
kamprogavrc -d -f tc77_soft.hex