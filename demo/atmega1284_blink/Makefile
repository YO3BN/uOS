
all:
	avr-gcc -mmcu=atmega1284 -DF_CPU=8000000 -I../../src/kernel/include -Os \
	main.c -o main ../../bin/kernel.a -fdata-sections -ffunction-sections	\
	-Wl,--gc-sections
	
debug:
	avr-gcc -mmcu=atmega1284 -DF_CPU=8000000 -I../../src/kernel/include -g	\
	-Og main.c -o main ../../bin/kernel.a

clean:
	rm main
	