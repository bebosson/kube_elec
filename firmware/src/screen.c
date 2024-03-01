#include "avr/io.h"


char buffer_text[8][21];
static char reload_screen = 0;

//576 octets (256 * 2 + 64)
const char font[50][6] = {
    {0,0,0,0,0,0},
    {124,10,9,10,124},      // A
    {127,73,73,78,48,0},    // B
    {62,65,65,65,34,0}, // C
    {127,65,65,34,28,0}, // D
    {127,73,73,73,65,0}, // E
    {127,9,9,9,1,0}, // F
    {62,65,73,73,58,0}, // G
    {127,8,8,8,127,0}, // H
    {0,65,127,65,0,0}, // I
    {0,32,65,63,1,0}, // J
    {127,8,20,34,65,0}, // K
    {127,64,64,64,64,0}, // L
    {127,2,12,2,127,0}, // M
    {127,6,8,48,127,0}, // N
    {62,65,65,65,62,0}, // O
    {127,9,9,9,6,0}, // P
    {62,65,81,33,94,0}, // Q
    {127,9,25,41,70,0}, // R
    {70,73,73,73,49,0}, // S
    {1,1,127,1,1,0}, // T
    {63,64,64,64,63,0}, // U
    {31,32,64,32,31,0}, // V
    {63,64,56,64,63,0}, // W
    {99,20,8,20,99,0}, // X
    {7,8,112,8,7,0}, // Y
    {97,81,73,69,67,0}, // Z
    {62,81,73,69,62,0}, // 0
    {0,66,127,64,0,0}, // 1
    {66,97,81,73,70,0}, // 2
    {33,65,69,75,49,0}, // 3
    {24,20,18,127,16,0}, // 4
    {71,69,69,69,57,0}, // 5
    {60,74,73,73,48,0}, // 6
    {1,113,9,5,3,0}, // 7
    {54,73,73,73,54,0}, // 8
    {6,73,73,41,30,0}, // 9
    {0,54,54,0,0,0}, // :
    {0,0,95,0,0,0}, // !
    {8,8,8,8,8,0}, // -
    {8,28,42,8,8,0}, // <-
    {8,8,42,28,8,0}, // ->
    {16,32,126,32,16,0}, // v
    {8,4,126,4,8,0}, // ^
    {0,0,126,0,0,0}, // |
    {0,0,96,48,0,0}, // ,
    {0,0,96,96,0,0}, // .
    {0,0,2,5,2,0}, // °
    {35,19,8,4,50,49} // %
};

uint8_t init_seq[] = {
    //start transfert,  /* enable chip, delay is part of the transfer start */
    0x0ae,              /* display off */
    // 0x0d5, 0x080,       /* clock divide ratio (0x00=1) and oscillator frequency (0x8) */
    // 0x0a8, 0x03f,       /* multiplex ratio */
    // 0x0d3, 0x000,       /* display offset */
    0x040,              /* set display start line to 0 */
    0x08d, 0x014,       /* [2] charge pump setting (p62): 0x014 enable, 0x010 disable, SSD1306 only, should be removed for SH1106 */
    0x020, 0x000,       /* page addressing mode */
    0x0a1,              /* segment remap a0/a1 */
    0x0c0,              /* c0: scan dir normal, c8: reverse */
    // 0x0da, 0x012,       /* com pin HW config, sequential com pin config (bit 4), disable left/right remap (bit 5) */
    // 0x081, 0x0cf,       /* [2] set contrast control */
    // 0x0d9, 0x0f1,       /* [2] pre-charge period 0x022/f1*/
    // 0x0db, 0x040,       /* vcomh deselect level */
    // 0x02e,              /* Deactivate scroll */ 
    0x0a4,              /* output ram to display */
    0x0a6,               /* none inverted normal display mode */
    //end transfert     /* disable chip */
    //end               /* end of sequence */
    0x0af,              // display on
};




void    put_str(char *str, char x, char y) {
    reload_screen = 1;
    char *text = &buffer_text[y][x];
    while (*str)
    {
        if (*str >= 'A' && *str <= 'Z')
            *text = *str - 'A' + 1;
        else if (*str >= '0' && *str <= '9')
            *text = *str - '0' + 27;
        else if (*str == ' ')
            *text = 0;
        else if (*str == ':')
            *text = 37;
        else if (*str == '!')
            *text = 38;
        else if (*str == '-')
            *text = 39;
        else if (*str == '<')
            *text = 40;
        else if (*str == '>')
            *text = 41;
        else if (*str == 'v')
            *text = 42;
        else if (*str == '^')
            *text = 43;
        else if (*str == '|')
            *text = 44;
        else if (*str == ',')
            *text = 45;
        else if (*str == '.')
            *text = 46;
        else if (*str == 'o')
            *text = 47;
        else if (*str == '%')
            *text = 48;
        str++;
        text++;
    }
}

void putnbr(unsigned long nb, char x, char y) {
    char *text = &buffer_text[y][x];
    reload_screen = 1;

    if (nb >= 1000)
    {
        *text++ = nb / 1000 + 27;
        if (nb % 1000 < 100) {
            *text++ = 27;
        }
        if (nb % 1000 < 10) {
            *text++ = 27;
        }
        nb %= 1000;
    }
    if (nb >= 100)
    {
        *text++ = nb / 100 + 27;
        if (nb % 100 < 10)
            *text++ = 27;
        nb %= 100;
    }
    if (nb >= 10)
    {
        *text++ = (nb / 10) % 10 + 27;
        nb %= 10;
    }
    if (nb >= 0)
        *text++ = nb + 27;
}

void SPI_MasterTransmit(char cData)
{
    /* Start transmission */
    SPDR1 = cData;
    /* Wait for transmission complete */
    while(!(SPSR1 & (1<<SPIF)))
    ;
}

void    clear_buffer() {
    for (int j = 0; j < 8; j++)
    {
        for (int i = 0; i < 21; i++)
            buffer_text[j][i] = 0;
    }
}

void    print_screen() {
    if (!reload_screen)
        return ;
    PORTD &= ~(1 << PD6); // set command mode
    SPI_MasterTransmit(0x00);
    SPI_MasterTransmit(0x10);
    PORTD |= (1 << PD6); // set data mode
    for (int l = 0; l < 8; l++) {
        uint8_t pixel_page[128];
        char *text = buffer_text[l];
        for (int j = 0; j < 21; j++)
        {
            uint8_t *c = font[text[j]];
            pixel_page[j*6] = c[0];     //0
            pixel_page[j*6+1] = c[1];   //7
            pixel_page[j*6+2] = c[2];   //14
            pixel_page[j*6+3] = c[3];   //21
            pixel_page[j*6+4] = c[4];   //28
            pixel_page[j*6+5] = c[5];   //35
        }
        pixel_page[126] = 0;
        pixel_page[127] = 0;
        for (int i = 127; i >= 0; i--)
        {
            SPI_MasterTransmit(pixel_page[i]);
        }
    }
    reload_screen = 0;
}

void    clear_screen() {
    reload_screen = 1;
    clear_buffer();
    print_screen();
}

void    display_init() {

    // set pin CS DC REST as output
    DDRD |= (1 << PD6); //DC
    DDRD |= (1 << PD7); //CS
    DDRB |= (1 << PB0); //RST
    // SPI_MasterInit
    PORTB |= (1 << PB2);
    SPCR1 = (1<<SPE)|(1<<MSTR);
    DDRD |= (1 << PD7);         // pinMode(SS, OUTPUT);
    DDRC |= (1 << PC1);         // pinMode(SCK, OUTPUT);
    DDRE |= (1 << PE3);         // pinMode(MOSI, OUTPUT);


    PORTB |= (1 << PB0); // pin reset sd1306
    PORTD &= ~(1 << PD6); // set command mode

    uint8_t cnt = 0; // sequence initialisation init_seq
    while (cnt < sizeof(init_seq)) {
        SPI_MasterTransmit(init_seq[cnt++]);
    }
    PORTD |= (1 << PD6); // set data mode
    for (int i = 0; i < 8*128; i++)
    {
        SPI_MasterTransmit(0); //clear screen
    }
    clear_buffer();
    
}

