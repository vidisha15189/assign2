#vidisha(2015189) and bhavna(2015136)

all: compile program

compile:
	avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o LED.o LED.c

	avr-gcc -mmcu=atmega328p LED.o -o LED.out

	avr-objcopy -O ihex LED.out LED.hex

	
program:
	sudo avrdude -c avrisp2 -p ATMEGA328P -P /dev/ttyUSB* -U flash:w:LED.hex


clean:
	-rm -rf *.o *.out *.hex

	
