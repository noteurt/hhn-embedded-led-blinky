#define F_CPU 20000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRD = 0xff;

	while(1)
	{
		PORTD = (0b11111111);
		_delay_ms(1000);
		PORTD = (0b000000);
		_delay_ms(1000);
	}
}