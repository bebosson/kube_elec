#include "main.h"
#include <string.h>


void eeprom_write ( uint16_t address, uint8_t byte ) {  //write a byte to the AVR EEPROM
        while (EECR & _BV(EEPE)) ;                 //wait until any write operation is complete
        EEAR = address;                            //set the desired address
        EEDR = byte;                               //set the desired byte
        EECR |= ( 1 << EEMPE );                    //enable write operations
        EECR |= ( 1 << EEPE );                     //start the write operation
}

uint8_t eeprom_read ( uint16_t address ) {     //read a byte from the AVR EEPROM
        while (EECR & _BV(EEPE)) ;                 //wait until any write operation is complete
        EEAR = address;                            //set the desired address
        EECR |= ( 1 << EERE );                     //trigger a read operation
        return EEDR;                               //return the data
}

void eeprom_write_page (uint16_t saddress, uint16_t n, uint8_t* bytes) {  //write a string of bytes to EEPROM
        uint16_t c;                                //counter variable
        for(c=0;c<n;c++) {                         //write n number of bytes
            eeprom_write(saddress+c, bytes[c]);    //write the next byte in the string
    }
}

void eeprom_read_page (uint16_t saddress, uint16_t n, uint8_t* bytes) {  //read a string of bytes from EEPROM
        uint16_t c;                                //counter variable
        for(c=0;c<n;c++) {                         //read n number of bytes
            bytes[c] = eeprom_read(saddress+c);    //read and store the next byte
    }
}

//set the first byte to 0 when it's already configure

void    init_species(uint8_t *species)
{
    // t_specie species;
    // char tmp[8] = "cactus\0\0";
    // species.name = tmp;
    // species.lux_max = 200;
    // species.lux_min = 120;
    // species.temp_max = 30;
    // species.temp_min = 24;
    // species.mois_max = 600;
    // species.mois_min = 300;
    // uint8_t spec1[18];
    // species[0] = 'C';
    // species[1] = 'A';
    // species[2] = 'C';
    // species[3] = 'T';
    // species[4] = 'U';
    // species[5] = 'S';
 
    
    // memcpy(species, "CACTUS\0\0", 8);
    // memcpy(species + 8, 0, 1);
    // memcpy(species + 9, 200, 1);
    // memcpy(species + 10, 0, 1);
    // memcpy(species + 11, 120, 1);
    // memcpy(species + 12, 30, 1);
    // memcpy(species + 13, 24, 1);
    // memcpy(species + 14, (uint8_t)(600 >> 8), 1);
    // memcpy(species + 15, (uint8_t)600, 1);
    // memcpy(species + 16, (uint8_t)(300 >> 8), 1);
    // memcpy(species + 17, (uint8_t)300, 1);
    // memcpy(species + 18, "PETUNIA\0", 8);
    // memcpy(species + 18 + 8, 0, 1);
    // memcpy(species + 18 + 9, 200, 1);
    // memcpy(species + 18 + 10, 0, 1);
    // memcpy(species + 18 + 11, 120, 1);
    // memcpy(species + 18 + 12, 30, 1);
    // memcpy(species + 18 + 13, 24, 1);
    // memcpy(species + 18 + 14, (uint8_t)(600 >> 8), 1);
    // memcpy(species + 18 + 15, (uint8_t)600, 1);
    // memcpy(species + 18 + 16, (uint8_t)(300 >> 8), 1);
    // memcpy(species + 18 + 17, (uint8_t)300, 1);
}


void eeprom_write_specie()
{
    uint8_t species[36];
                                    // {"Dalia\0\0\0", 200, 120, 30, 24, 600, 300},
                                    // {"Monarda\0", 200, 120, 30, 24, 600, 300}};

    init_species(species);
    // for (int i = 0; i < 36; i++)
    // {
    //     eeprom_write(i, 1);
    // }
    eeprom_write_page(0, 36, species);

}

