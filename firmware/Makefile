CC=avr-gcc
OBJCPY=avr-objcopy
CFLAGS= -std=c99 -Wall -ffunction-sections
DUDE = avrdude
F_CPU=8000000
PORT=/dev/cu.usbserial-FTHHXGCT
MCU = atmega328pb 

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

EXE := $(BIN_DIR)/exec
SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

LDLIBS := -lm
LDFLAGS  := -Llib

CDEFS = -DF_CPU=$(F_CPU)UL
ALL_CFLAGS = $(CFLAGS) -Iinclude -mmcu=$(MCU) $(CDEFS) -O1 #-Os ou O3 ?
SCREEN_PID := $(shell ps -t cu.usbserial-FTHHXGCT | tail -n1 | awk '{print $$1}')

all: $(EXE).hex flash

$(EXE).bin : $(OBJ)
	@mkdir -p $(BIN_DIR)
	@$(CC) $(LDFLAGS) $(ALL_CFLAGS) $^ $(LDLIBS) -o $@


$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(ALL_CFLAGS) -c $< -o $@

$(EXE).hex: $(EXE).bin
	@$(OBJCPY) -j .text -j .data -O ihex $(EXE).bin $(EXE).hex
	@avr-size --format=avr --mcu=$(MCU) $(EXE).bin
flash:
		@$(DUDE) -v -p $(MCU) -c arduino -P $(PORT) -b 57600 -U flash:w:$(EXE).hex:i
clean:
	rm -rf $(OBJ_DIR)
fclean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)
re: fclean all
ks:
	kill -3 $(SCREEN_PID)

reks: ks all

.PHONY: all flash clean
