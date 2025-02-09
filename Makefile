all:
	avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Wall -Os -o main.elf main.c
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
	rm main.elf
	#avrdude -c avrisp -b 19200 -p m328 -P /dev/ttyACM0 -U lfuse:w:0x2F:m
	avrdude -c avrisp -b 19200 -p m328 -P /dev/ttyACM0 -U flash:w:main.hex
