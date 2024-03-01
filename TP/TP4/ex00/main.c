#include <avr/io.h>

void ft_putnbr(unsigned int nb);

unsigned int power(unsigned int nb, int power) {
	unsigned int val = nb;
	while (--power) {
		val *= nb;
	}
	return val;
}

void uart_init() {
	//UCSR0A &= ~(0 << U2X0);
	UCSR0A |= (1 << U2X0);
	/*Set baud rate to 115200 */
	UBRR0L = 16; //2^4 = 16
	/*Enable transmitter */
	UCSR0B |= (1 << TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01); //8data
}

void uart_tx(char c) {
	while ( !( UCSR0A & (1<<UDRE0)) ); // wait for empty buffer
	UDR0 = c;
}

void TIMER1_COMPA_vect() __attribute__((signal));
void TIMER1_COMPA_vect() {
	unsigned int nb = 0;
	uart_tx(' ');
	uart_tx('|');
	uart_tx('|');
	uart_tx(' ');
	ADCSRA |= (1 << ADSC); // start calculations
	while(ADCSRA & (1 << ADSC)); // wait calculations is done
	nb = (unsigned int)ADCL;
	if (ADCH & 1) {
		nb |= (1 << 8);
	}
	if (ADCH & 2) {
		nb |= (1 << 9);
	}
	ft_putnbr(nb);
}

void ft_putnbr(unsigned int nb) {
	if (nb >= 10)
	{
		ft_putnbr(nb / 10);
		ft_putnbr(nb % 10);
	}
	else
		uart_tx(nb + '0');
}

int main() {
	ADCSRA |= (1 << ADEN); // enable ADC
	ADMUX |= (1 << REFS0); // VCC -> voltage reference
	uart_init();
	SREG |= (1 << SREG_I); // allow interrupts
	TIMSK1 |= (1 << OCIE1A); // enable Compare A Match interrupt
	TCCR1B |= (1 << WGM12); // set CTC mode
	OCR1A = 2 * F_CPU / (256 * 2) - 1; // 2 sec
	TCCR1B |= (1 << CS12); // prescaler 256
	for(;;) {
	}
}
