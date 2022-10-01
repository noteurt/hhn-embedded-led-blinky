#define F_CPU 20000000UL
#include <avr/io.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
char statusPORTD = 0b11111111;

int main(void)
{
	//initialize output leds
	DDRB = 0xff;
	//initialize input switches
	DDRD = 0x00;
	
	
	PCICR |= (1 << PCIE3);
	
	PCMSK3 |= 0b11111111 ;

	
	PORTB = (0b11111111);
	
	sei();   // Enable global interrupts by setting global interrupt enable bit in SREG
	

	while(1)
	{
		check_if_button_is_pressed();

	}
}

void check_if_button_is_pressed(){
	switch(statusPORTD)
	{
		case 0b11111110:
		do
		{
			PORTB = (0b11111111);
			_delay_ms(100);
			PORTB = (0b10101010);
			_delay_ms(100);
		} while (statusPORTD == 0b11111110);
		PORTB = (0b11111111);
		break;
		
		case 0b11111101:
		do
		{
			for (int i=0;(i<8) && (statusPORTD == 0b11111101);i++)
			{
				PORTB = (0b11111110 << i);
				_delay_ms(100);
			}
			
		} while (statusPORTD == 0b11111101);
		PORTB = (0b11111111);
		break;
		
		case 0b11111011:
		do
		{
			PORTB = (0b11111111);
			_delay_ms(10);
			PORTB = (0b00000000);
			_delay_ms(10);
		} while (statusPORTD == 0b11111011);
		PORTB = (0b11111111);
		break;
		
		case 0b11110111:
		PORTB = statusPORTD;
		break;
		
		//leds ausschalten, indem man hart-code das kein Button gedrückt wurde
		case 0b01111111:
		set_sleep_mode(SLEEP_MODE_IDLE);
		sleep_mode();
		break;
		
		default:;
		
		// default statements
	};
}

ISR(PCINT3_vect){
	//statusPORTD = PORTD;
	
	unsigned char temp1, temp2;
	
	temp1 = PIND;                  // read input
	_delay_ms(5);                  // delay for key debounce
	temp2 = PIND;
	/*
	if abfrage speichert nur dann das Ergebnis,
	wenn es nicht "0b11111111" ist
	weil der Interrupt "feuert" sich einmal bei
	drücken der Taste und einemal beim loslassen
	der Taste, beim loslassen ist PORTD = "0b11111111"
	*/
	if((temp1 & temp2) != 0b11111111){
		statusPORTD = (temp1 & temp2);
	}
	
}