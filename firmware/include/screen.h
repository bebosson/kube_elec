#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <inttypes.h>



typedef struct  s_screen {
    uint8_t     buffer[128];
	uint8_t		buf_height;
	uint8_t		tile_curr_row;
}               t_screen;

typedef struct s_specie{
    char    *name;
    uint16_t lux_max;
    uint16_t lux_min;
    uint8_t temp_max;
    uint8_t temp_min;
    uint16_t mois_max;
    uint16_t mois_min;
}           t_specie;

void    putnbr(unsigned long nb, char x, char y);
void    put_str(char *str, char x, char y);
void	SPI_MasterTransmit(char cData);
void    display_init();
void    clear_screen();
void    print_screen();
void    clear_buffer();

#endif