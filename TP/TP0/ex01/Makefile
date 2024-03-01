CC=avr-gcc
OBJCPY=avr-objcopy
CFLAGS= -std=c99 -Wall -ffunction-sections
DUDE = avrdude
F_CPU=16000000
PORT=/dev/cu.SLAB_USBtoUART
MCU = atmega328p
SRC = main
CDEFS = -DF_CPU=$(F_CPU)UL
ALL_CFLAGS = $(CFLAGS) -I. -mmcu=$(MCU) $(CDEFS)

all: hex flash
hex : 
		$(CC) $(ALL_CFLAGS) -c $(SRC).c -o $(SRC).o
		$(CC) $(ALL_CFLAGS) -o $(SRC).bin  $(SRC).o
		$(OBJCPY) -j .text -j .data -O ihex $(SRC).bin $(SRC).hex
		avr-size --format=avr --mcu=$(MCU) $(SRC).bin
flash :
		$(DUDE) -v -p $(MCU) -c arduino -P $(PORT) -b 115200 -U flash:w:$(SRC).hex:i
clean:
	rm $(SRC).o
	rm $(SRC).bin
	rm $(SRC).hex
