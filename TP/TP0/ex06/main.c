#include <avr/io.h>

int main()
{
	PORTB = 0;
	int i = 0;

	DDRB = 15;
	for (;;)
	{
		if (PIND & (1 << 3))
			PORTB += i;
		else if (PIND ^ (1 << 3))
			i = 1;
		else if (PORTB & 15)
			i = 0;
	}
}
