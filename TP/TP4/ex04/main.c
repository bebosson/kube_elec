#include <avr/io.h>

void uart_init() {
	/*Set baud rate to 8 */
	UBRR0L = (1<<3); //2^3 = 8
	/*Enable transmitter and receiver*/
	UCSR0B = (1<<TXEN0) | (1 << RXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (1<<UCSZ00) | (1 << UCSZ01); //8data
}

void uart_tx(char c) {
	UDR0 = c; // transmit char
}

void uart_printstr(const char* str) {
	while (*str) {
		/* Wait for empty transmit buffer */
		while (!(UCSR0A & (1 << UDRE0)));
		uart_tx(*str);
		str++;
	}
}

char uart_rx(void) {
	/* Wait for data to be received */
	while (!(UCSR0A & (1<<RXC0)));
	return (UDR0);
}

void uart_tx_new(char c) {
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

void wait_for_enter(char *str, int32_t length, char *too_long, char hide, int32_t init_length) {
	/* Wait for data to be received */
	while(!(UCSR0A & (1<<RXC0)));
	char c = UDR0;
	if (c != 13) { // if char is not Enter
		if ( c != 127) { // if char is not Backspace
			if (hide == 1) {
				uart_tx_new('*');
			} else {
				uart_tx_new(c);
			}
			if (length > 0) { //length -> nb of char awaited left
				*str = c;
				wait_for_enter(str + 1, length - 1, too_long, hide, init_length);
			} else { // no char left awaited
				*too_long = 1; // str is longer than expected
				wait_for_enter(str, length - 1, too_long, hide, init_length);
			}
		} else { // if char is backspace
			if (length == init_length) { // if no char written for now
				wait_for_enter(str, length, too_long, hide, init_length);
			} else if (length < init_length && length >= 0) { // if we erase important char
				uart_tx_new(8);
				uart_tx_new(' ');
				uart_tx_new(8);
				*(str - 1) = '\0';
				*too_long = 0;
				wait_for_enter(str - 1, length + 1, too_long, hide, init_length);
			} else { // if we erase extra char
				uart_tx_new(8);
				uart_tx_new(' ');
				uart_tx_new(8);
				if (length + 1 == 0) {
					*too_long = 0; // the string is not too long anymore
				}
				wait_for_enter(str, length + 1, too_long, hide, init_length);
			}
		}

	}
}

void init_empty_str(char *str, int length) {
	int i = 0;
	while(i < length) {
		str[i++] = '\0';
	}
}

void login(char *too_long, char *username, int32_t u_length, char *password, int32_t p_length) {
	*too_long = 0;
	char username_was_too_long = 0;
	init_empty_str(username, u_length + 1);
	init_empty_str(password, p_length + 1);
	uart_printstr("Bonjour ! Entrez votre Login:\r\n");
	uart_printstr("    username: ");
	wait_for_enter(username, u_length, too_long, 0, u_length);
	username_was_too_long = *too_long;
	//uart_printstr("\r\n /debug/ ");
	//uart_tx_new(username_was_too_long + 48);
	//uart_printstr(" /debug/\r\n ");
	uart_printstr("\r\n    password: ");
	wait_for_enter(password, p_length, too_long, 1, p_length);
	if (username_was_too_long) {
		*too_long = 1;
	}
}

char unequal_str(const char *str1, char *str2) {
	while (*str1) {
		if (*str1 != *str2) {
			return (1);
		}
		str1++;
		str2++;
	}
	return (0);
}

int main() {
	uart_init();
	char logged = 0;
	char too_long;
	const char username[] = "pimichau";
	const char password[] = "1234";
	char typed_username[9];
	char typed_password[5];
	while (logged == 0) {
		login(&too_long, typed_username, 8, typed_password, 4);
		if (too_long || unequal_str(username, typed_username) || unequal_str(password, typed_password)) {
			uart_printstr("\r\nMauvaise combinaison username/password\r\n\r\n");
		//	uart_printstr("debug:\r\n");
		//	uart_printstr(typed_username);
		//	uart_printstr("\r\ndebug:\r\n");
		//	uart_printstr(typed_password);
		//	uart_printstr("\r\ndebug:\r\n");
		//	uart_tx_new(unequal_str(username, typed_username) + 48);
		//	uart_printstr(" // ");
		//	uart_tx_new(unequal_str(password, typed_password) + 48);
		//	uart_printstr(" // ");
		//	uart_tx_new(too_long + 48);
		//	uart_printstr("\r\nfin debug\r\n\r\n");
		} else {
			logged = 1;
			uart_printstr("\r\nCoucou pimichau !\r\n\r\n");
		}
	}
}
