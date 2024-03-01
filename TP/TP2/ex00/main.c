#include <avr/io.h>

int main(void)
{
    DDRD &= ~(1 << DDD3);     // Clear the PD2 pin
    // PD2 (PCINT0 pin) is now an input

    PORTD |= (1 << PORTD2);    // turn On the Pull-up
    // PD2 is now an input with pull-up enabled



    EICRA |= (1 << ISC01);    // set INT0 to trigger on ANY logic change
    EIMSK |= (1 << INT1);     // Turns on INT0


    while(1)
    {
        /*main program loop here */
    }
}



ISR (INT0_vect)
{
    /* interrupt code here */
}
