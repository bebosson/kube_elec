#include <avr/io.h>

int main()
{
	DDRB |= (1 << DDB1); 
	TCCR1B = (1 << CS10) | (1 << CS12) | (1 << WGM12) | (1 << WGM13);
	TCCR1A = (1 << COM1A1) | (1 << WGM11); 
	ICR1 = F_CPU / (1024) - 1;
	OCR1A = (F_CPU / (1024) - 1) * 0.1;
	for(;;)
	{

	}
}
