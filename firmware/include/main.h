#include <stdbool.h>
#include <inttypes.h>
#include "twi.h"
#include "utils.h"
#include "screen.h"
#include "eeprom.h"
#include "avr/io.h"
#include "esp.h"


#define DEFAULT_I2C_ADDRESS 0x48;
#define BUFFER_LENGTH 32
#define TWI_READY 0
#define TWI_MRX   1
#define TWI_MTX   2
#define TWI_SRX   3
#define TWI_STX   4

typedef struct  s_I2C_Data {
    bool    transmitting;
    uint8_t txAddress;
    uint8_t txBufferIndex;
    uint8_t txBufferLength;
    uint8_t txBuffer[];
}               t_I2C_Data;
 