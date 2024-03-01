NAME=main

F_CPU=16000000UL
MCU=atmega328p
# we want to override this if we have our own udev rule with a different name
PORT ?= /dev/tty.SLAB_USBtoUART

CC = avr-gcc
SRC = main.c
CFLAGS ?= -std=c99 -Wall -ffunction-sections
SCREEN_PID := $(shell ps -t tty.SLAB_USBtoUART | tail -n1 | awk '{print $$1}')

# all : hex flash considered harmful (race condition with -j)
all:
	$(MAKE) hex
	$(MAKE) flash

$(NAME).bin: $(SRC)
	$(CC) $(CFLAGS) -I. -mmcu=$(MCU) -DF_CPU=$(F_CPU) -o $(NAME).bin $(SRC)

$(NAME).hex: $(SRC)
	avr-size $(NAME).bin
	avr-objcopy -j .text -j .data -O ihex main.bin main.hex

hex:
	$(MAKE) $(NAME).bin
	$(MAKE) $(NAME).hex

flash:
	avrdude -p $(MCU) -c arduino -P $(PORT) -b 57600 -D -U flash:w:main.hex:i

clean:
	$(RM) $(NAME).bin $(NAME).hex

ks:
	kill -3 $(SCREEN_PID)

reks: ks all
.PHONY: all hex flash clean
