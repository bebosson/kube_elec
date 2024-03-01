#include <avr/io.h>

void uart_init() {
	/*Set baud rate to 8 */
	UBRR0L = (1 << 3); //2^3 = 8
	/*Enable transmitter and receiver*/
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01); //8data
}

void uart_tx(char c) {
	UDR0 = c;
}

char uart_rx(void) {
	/* Wait for data to be received */
	while ( !(UCSR0A & (1<<RXC0)) );
	return (UDR0);
}

int main() {
	uart_init();
	for(;;) {
		uart_tx(uart_rx());
	}
}
