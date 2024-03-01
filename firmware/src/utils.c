#include <inttypes.h>
#include <avr/io.h>
#include "screen.h"

void    ft_delay(uint64_t delay) {
    while (delay--){};
}

void uart_init() {
	// UCSR0A |= (1 << U2X0); // double the speed rate
	/*Set baud rate */
	UBRR0L = 51;
	/*Enable transmitter */
	UCSR0B |= (1 << TXEN0);
    UCSR0B |= (1 << RXEN0); 
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01); //8data
}

void uart_tx(char c) {
	while ( !( UCSR0A & (1<<UDRE0)) ); // wait for empty buffer
	UDR0 = c;
}

void uart_strx(char *str) {
    while (*str) {
        uart_tx(*str);
        str++;
    }
}

// char uart_rx()
// {
//     /* Wait for data to be received */
//     while (!(UCSR0A & (1 << RXC)));
//     /* Get and return received data from buffer */
//     return UDR0;
// }

void uart_init1() {
	// UCSR0A |= (1 << U2X0); // double the speed rate
	/*Set baud rate */
	UBRR1L = 51; // 8 -> 115200 | 16 -> 57600
	/*Enable transmitter */
	UCSR1B |= (1 << TXEN1);
	/* Set frame format: 8data, 1stop bit */
	UCSR1C = (1 << UCSZ10) | (1 << UCSZ11); //8data
}

void uart_tx1(char c) {
	while ( !( UCSR1A & (1<<UDRE1)) ); // wait for empty buffer
	UDR1 = c;
}

void uart_strx1(char *str) {
    while (*str) {
        uart_tx1(*str);
        str++;
    }
}

void uart_putnbr(uint16_t nb) {
	if (nb >= 10)
	{
		uart_putnbr(nb / 10);
		uart_putnbr(nb % 10);
	}
	else
		uart_tx(nb + '0');
}

void uart_putnbr8(uint8_t nb) {
	if (nb >= 10)
	{
		uart_putnbr8(nb / 10);
		uart_putnbr8(nb % 10);
	}
	else
		uart_tx(nb + '0');
}

void ft_uitoa(uint16_t nb, char *str, uint8_t len)
{
    if (len > 0 && str) {
        str[len - 1] = '\0';
        if (*str) //Cas ou il y a un "-" (temperature negative)
            str++;
        if (nb == 0) {
            str[0] = '0';
            return ;
        }
        int j = 1;
        if (nb > 9)
            j = nb < 100 ? 2 : 3;
        if (nb >= 1000)
            j = 4;
        while (--j >= 0)
        {
            str[j] = (nb % 10) + 48;
            nb /= 10;
        }
    }
}

char *append_str(char *dest, char *content) {
    while(*dest) {
        dest++;
    }
    while(*content) {
        *dest = *content;
        dest++;
        content++;
    }
    return (dest);
}


void init_str(char *dest, uint8_t len) {
    for (int i = 0; i < len; i++) {
        dest[i] = '\0';
    }
}