#include <avr/io.h>

void uart_init() {
	/*Set baud rate to 115200 */
	UBRR0L |= (1 << 3); //2^3 = 8
	/*Enable transmitter */
	UCSR0B |= (1 << TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01); //8data
}

void uart_tx(char c) {
	UDR0 = c;
}

void uart_printstr(const char* str) {
	while (*str) {
		/* Wait for empty transmit buffer */
		while (!(UCSR0A & (1 << UDRE0)));
		uart_tx(*str);
		str++;
	}
}

__attribute__ ((signal)) void TIMER1_COMPA_vect() {
	const char str[] = "Hello World!\r\n";
	uart_printstr(str);
}

int main() {
	uart_init();
	SREG |= (1 << SREG_I); // allow interrupts
	TIMSK1 |= (1 << OCIE1A); // enable Compare A Match interrupt
	TCCR1B |= (1 << WGM12); // set CTC mode
	OCR1A = 2 * (F_CPU / 1024 - 1); // 2 sec
	TCCR1B |= (1 << CS12) | (1 << CS10); // prescaler 1024
	for(;;);
}
