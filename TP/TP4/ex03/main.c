#include <avr/io.h>

void uart_init() {
	/*Set baud rate to 8 */
	UBRR0L = 103; //2^3 = 8
	/*Enable transmitter and receiver*/
	UCSR0B = (1<<TXEN0) | (1 << RXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (1<<UCSZ00) | (1 << UCSZ01); //8data
}

void uart_tx(char c) {
	UDR0 = c;
}

char uart_rx(void) {
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)));
	return (UDR0);
}

__attribute__ ((signal)) void USART_RX_vect() {
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1<<UDRE0)));
	char c = UDR0;
	if (c >= 'a' && c <= 'z') {
		uart_tx(c - 32);
	} else if (c >= 'A' && c <= 'Z') {
		uart_tx(c + 32);
	} else {
		uart_tx(c);
	}
}

int main() {
	uart_init();
	UCSR0B |= (1<<RXCIE0); // enable interrupts when data received
	SREG |= (1<<SREG_I); // enable interrupts
//	for (int i=0; i < F_CPU / 500; i++){};
	for(;;) {
		while (!(UCSR0A & (1<<UDRE0)));
 		UDR0 = 'a';
	//	for (int i=0; i < F_CPU / 500; i++){};
	}
}
