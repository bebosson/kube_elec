

#include <avr/io.h>


int main()
{
	DDRD |= (1 << 4);
	COM1Ax = 0x00000101;
	while(1)
	{

	}
}
