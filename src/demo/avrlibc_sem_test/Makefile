
all:
	avr-gcc -pipe -gstabs -static -O2 -mmcu=atmega1284 -DF_CPU=8000000 \
	-fdata-sections -ffunction-sections -Wall -Wl,--gc-sections,-u, \
	-I../../kernel/include \
	-I../../arch/avr8/atmega1284 \
	-I../../demo/avrlibc_sem_test \
	main.c -o main \
	../../../src/kernel/*.c \
	../../../src/arch/avr8/atmega1284/*.c

	avr-size main

debug:
	avr-gcc -pipe -gstabs -static -O0 -g -mmcu=atmega1284 -DF_CPU=8000000 \
	-fdata-sections -ffunction-sections -Wall -Wl,--gc-sections,-u, \
	-I../../kernel/include \
	-I../../arch/avr8/atmega1284 \
	-I../../demo/avrlibc_sem_test \
	main.c -o main \
	../../../src/kernel/*.c \
	../../../src/arch/avr8/atmega1284/*.c

	avr-size main

clean:
	rm main
