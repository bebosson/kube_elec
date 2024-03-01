#include <avr/io.h>
int main()
{
	PORTB = 0;
	DDRB |= (1 << 3);
	int i = 0;
	for (;;)
	{
		if (PIND & (1 << 3))
			PORTB = i;
		else if (PIND ^ (1 << 3) && PORTB == 0) // ? 
			i = (1 << 3);
		else if (PIND ^ (1 << 3) && PORTB == (1 << 3))
			i = 0;
	}
}
