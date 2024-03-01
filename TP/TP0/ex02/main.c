#include <avr/io.h>

int main()
{
	DDRB |= (1 << 3);
	PORTB ^= (1 << 3);
	return(0);
}
