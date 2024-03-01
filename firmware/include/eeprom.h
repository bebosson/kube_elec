#ifndef _EEPROM_H
#define _EEPROM_H 1

#include "avr/io.h"
#include <stdint.h>

void eeprom_write ( uint16_t address, uint8_t byte );
uint8_t eeprom_read ( uint16_t address );
void eeprom_write_page (uint16_t saddress, uint16_t n, uint8_t* bytes);
void eeprom_read_page (uint16_t saddress, uint16_t n, uint8_t* bytes);

void eeprom_write_specie();

#endif
