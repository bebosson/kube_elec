#include <avr/io.h>
int main()
{
	DDRB |= (1 << 3);
//	PIND = 0;
	DDRD &= (0 << 3);
	for (;;)
	{
		if (PIND & (1 << 3)) // ? 
			PORTB = 0;
		else if (PIND ^ (1 << 3))
			PORTB ^= (1 << 3);
	}
}
