#include <avr/io.h>

int main()
{
	int a;
	long int j = F_CPU / 50;
	long int i = j;
	a = 0;
	for (;;)
	{
		DDRB |= (1 << 3); 
		PORTB ^= (1 << 3);
//		a += (a > 3) ? 0 : 1;
		while (i--);
		i = j;
	}
	return(0);
}
