#ifndef F_CPU
#define F_CPU 16000000UL         // 16 MHz clock speed
#endif

#include<util/delay.h>

int main(void)
{
    *((volatile unsigned char *)0x24) = 0xFF;                //Nakes PORTB as Output
    while(1)                    //infinite loop
    {
        *((volatile unsigned char *)0x25) = 0xFF;           //Turns ON All LEDs
        _delay_ms(1000);       //1 second delay
        *((volatile unsigned char *)0x25) = 0x00;            //Turns OFF All LEDs
        _delay_ms(1000);        //1 second delay
    }
}


