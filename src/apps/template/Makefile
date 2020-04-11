############################################################################
#
#	Temporary Makefile for applications.
#
#	Cristian Ionita
#	2 Apr 2020
#
############################################################################


MCU := atmega1284
FREQ := 1000000
MCUDIR := avr

APP_NAME := template

APP_INCLUDE := -I.
APP_SOURCE := ./*.c

ARCH_INCLUDE := -I../../arch/$(MCUDIR)/$(MCU)/
ARCH_SOURCE := ../../../src/arch/$(MCUDIR)/$(MCU)/*.c

KERNEL_INCLUDE := -I../../kernel/include
KERNEL_SOURCE := ../../../src/kernel/*.c

DRIVER_INCLUDE := 
DRIVER_SOURCE := 



all:
	avr-gcc -pipe -static -O2 -mmcu=$(MCU) -DF_CPU=$(FREQ) \
	-fdata-sections -ffunction-sections -Wall -fomit-frame-pointer -fshort-enums -Wl,--gc-sections,-u, \
	$(KERNEL_INCLUDE)	\
	$(ARCH_INCLUDE)		\
	$(DRIVER_INCLUDE)	\
	$(APP_INCLUDE)		\
	$(KERNEL_SOURCE)	\
	$(ARCH_SOURCE)		\
	$(DRIVER_SOURCE)	\
	$(APP_SOURCE)		\
	-o $(APP_NAME)

	avr-size $(APP_NAME)
	avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex $(APP_NAME) \
	$(APP_NAME).hex



debug:
	avr-gcc -pipe -ggdb3 -static -O0 -g -pg -mmcu=$(MCU) -DF_CPU=$(FREQ) \
	-Wall \
	$(KERNEL_INCLUDE)	\
	$(ARCH_INCLUDE)		\
	$(DRIVER_INCLUDE)	\
	$(APP_INCLUDE)		\
	$(KERNEL_SOURCE)	\
	$(ARCH_SOURCE)		\
	$(DRIVER_SOURCE)	\
	$(APP_SOURCE)		\
	-o $(APP_NAME)

	avr-size $(APP_NAME)
	avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex $(APP_NAME) \
	$(APP_NAME).hex
	
	
	
clean:
	rm $(APP_NAME) $(APP_NAME).hex
	find . -name '*.o' -exec rm '{}' \;
	find . -name '*.a' -exec rm '{}' \;
