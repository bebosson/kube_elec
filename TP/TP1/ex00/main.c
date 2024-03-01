#include <avr/io.h>

int main()
{
	DDRB |= (1 << DDB1);  
	TCCR1B = (1 << CS10) | (1 << CS12) | (1 << WGM12);
	TCCR1A |= (1 << COM1A0);
	OCR1A = F_CPU / (1024 * 2) - 1;
	for(;;)
	{

	}
}
