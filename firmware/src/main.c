#include "main.h"
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

// extern uint8_t      font[50][6];
char water_pump = 0;
volatile uint8_t    chosen_specie; // =1 if a specie is chosen otherwise =0
volatile uint8_t    menuid = 1;
volatile uint8_t    btn_press = 0;

uint8_t   clean_transition()
{
    for (uint8_t i = 0; i < 8; i++)
        put_str("                    ", 0, i);
    return (1);
}

void    transition_menu(uint8_t *x, uint8_t *y)
{
    if (*x == 8)
        *x = 19;
    else if (*x == 19)
        *x = 8;
}



void   navigation_button(uint8_t *x, uint8_t *y)
{

    if (PIND & (1 << PD4)) //boutton du bas
    {
        put_str(" ",*x, *y);
        (*y)++;
        if (*y > 7)
        {
            transition_menu(x, y);
            *y = 3;
        }
        ft_delay(F_CPU / 500);
//        PIND ^= ~(1 << PD4); rebond ?
    }
    if (PIND & (1 << PD5)) // boutton du bas
    {
        put_str(" ",*x, *y);
        (*y)--;
        if (*y < 3)
        {
            transition_menu(x, y);
            *y = 7;
        }
//        PIND ^= ~(1 << PD5); rebond ?
        ft_delay(F_CPU / 500);
    }
}

void    render_menu()
{
    uint8_t spec[8];
    put_str("CHOISISSEZ VOTRE", 2, 0);
    put_str("ESPECE", 7, 1);
    eeprom_read_page(0, 8, spec);
    put_str((char *)spec,0,3);
    eeprom_read_page(18, 8, spec);
    put_str((char *)spec,0,4);
    // eeprom_read_page(18, 18, specie);
    // put_str(specie,0,4);
    // eeprom_read_page(36, 18, specie);
    // put_str(specie,0,5);
    // put_str("CHAMP 4   ",0,6);
    // put_str("CHAMP 5   ",0,7);
}

void    display_menu() //choix espece
{
    static uint8_t x = 8;
    static uint8_t y = 3;
    navigation_button(&x, &y);
    render_menu();
    put_str("<", x, y);
    // if (PIND & (1 << PORT3))
    // {
        // ft_delay(F_CPU / 1000);
        chosen_specie = (y - 2) + (x == 18 ? 5 : 0); // CHOOSE_SPECIES
        // return;
    // }
    print_screen();
    return ;
}



uint16_t  getTemperature() {

    uint8_t tx_address = DEFAULT_I2C_ADDRESS;
    uint8_t rx_data[TWI_BUFFER_LENGTH];

    uint8_t i = 0;
    while (i < TWI_BUFFER_LENGTH) {
        rx_data[i++] = 0;
    }
    twi_writeTo(tx_address, 0x00, 1, true, true); //get temperature
    twi_readFrom(tx_address, rx_data, 2, true); //receive temperature
    twi_writeTo(tx_address, 0, 0, true, true); //end of communication

    return ((uint16_t)rx_data[0] << 8) | (rx_data[1]);
}

void    print_temp(int16_t val, char *g_temp) {
    init_str(g_temp, 6);
    put_str("TEMPERATURE:         ", 0, 4);
    if (val < 0) {

        put_str("-", 13, 4);
        g_temp[0] = '-';
    }
    val &= ~(1 << 15); // clear neg bit
    putnbr(val >> 8, 14, 4);
    ft_uitoa(val >> 8, g_temp, 6);
    put_str(".", 15 + (val >> 8 >= 10) + (val >> 8 >= 100), 4);
    putnbr(125 * ((val >> 5) & 7) / 100, 16 + (val >> 8 >= 10) + (val >> 8 >= 100), 4);
    ft_uitoa(125 * ((val >> 5) & 7) / 100, append_str(g_temp, "."), 2);
    put_str("oC", 17 + (val >> 8 >= 10) + (val >> 8 >= 100), 4);
}

uint16_t read_lux() {
    uint16_t lux = 0;
    ADMUX &= ~(1 << MUX3);//set pin A3
    ADMUX &= ~(1 << MUX2);
    ADMUX |= (1 << MUX1);
    ADMUX |= (1 << MUX0);
    ADCSRA |= (1 << ADSC); // start calculations
        while((ADCSRA & (1 << ADSC))); // wait calculations is done
        lux = ADCL | (uint16_t)(ADCH & 0b11) << 8;
    
    return (lux);
}

uint16_t read_moisture(){
    uint16_t mois = 0;
    uint16_t v_sec = 896;
    uint16_t v_humide = 448;
    uint16_t diff = v_sec - v_humide;

    ADMUX |= (1 << MUX1); // set ADC2
    ADMUX &= ~(1 << MUX0);
    ADMUX &= ~(1 << MUX2);
    ADMUX &= ~(1 << MUX3);
    ADCSRA |= (1 << ADSC); // start calculations
    while((ADCSRA & (1 << ADSC))); // wait calculations is done
    mois = ADCL | (uint16_t)(ADCH & 0b11) << 8;
    mois = ( v_sec - mois ) * 100 / diff;
    water_pump = (mois < 25) ? 1 : 0; 

    // putnbr(adch, 0, 4);
    // print_screen();
    // return (mois);
    // return ((100 - ((mois * 100 / 1023) / 100)));
    // return (mois);
//    return ((1023 - mois) / 5);
      return (mois);
}

__attribute__ ((signal)) void INT1_vect()
{
    btn_press = 1;
}

void    display_info(char *g_lux, char *g_mois, char *g_temp)
{
    uint8_t     memory_species = 18 * (chosen_specie - 1);
    uint8_t     spec[8];
    uint16_t    lux;
    uint16_t    mois;

    lux = read_lux(g_lux);
    put_str("LUMINOSITE:         ", 0, 0);
    putnbr(lux, 13, 0);
    init_str(g_lux, 5);
    ft_uitoa(lux, g_lux, 5);
    mois = read_moisture();
    put_str("HUMIDITE:       % ", 0, 2);
    putnbr(mois, 13, 2);
    init_str(g_mois, 5);
    ft_uitoa(mois, g_mois, 5);
    print_temp(getTemperature(), g_temp);
    put_str("ESPECE:   ", 0, 6);
    eeprom_read_page(memory_species, 8, spec); // recuperer dans la memoire l'espece choisi
    put_str((char *)spec, 11, 6);
    print_screen();
}


int       main() {
    uint16_t cptr = 0;
    char g_lux[5] = "\0\0\0\0";
    char g_mois[5] = "\0\0\0\0";
    char g_temp[6] = "\0\0\0\0\0";
    EICRA |= (1 << ISC10) | (1 << ISC11);
    SREG |= 1 << SREG_I; //allows interrupt
    EIMSK |= (1 << INT1); //enable INT1 for Select button press
    ADCSRA |= (1 << ADEN); // enable ADC
    ADMUX |= (1 << REFS0); // AVCC with external capacitor at AREF pin
    DDRD |= (1 << PD2); // POMPE
    PORTD |= (1 << PD2);
    ft_delay(F_CPU / 50);
    display_init(); //pin connected to screen
    // eeprom_write_specie();
    uart_init();
    ft_delay(F_CPU / 50);
    while(1) {
        if (btn_press) {
            clear_buffer();
            menuid = !menuid;
            _delay_ms(150);
            btn_press = 0;
        }
        if (menuid == 1)
            display_menu();
        else {
            display_info(g_lux, g_mois, g_temp);
            if (cptr++ == 1800) {
                put_str("SENDING!", 0, 7);
                print_screen();
                esp_send_data(g_lux, g_temp, g_mois);
                put_str("SENT!   ", 0, 7);
                print_screen();
                cptr = 0;
            }
        }
        ft_delay(F_CPU / 1000); 
    }
    return 0;
}