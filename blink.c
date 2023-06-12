#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

#define BLINK_MS 150


int main (){

	//DDRB |= 0b00100000;
	//Put B5 in output mode
	DDRB |= _BV(PORTB5);

while (true){
	// toggle the led
	PORTB ^= _BV(PORTB5);
	_delay_ms(BLINK_MS);
	/*
	// turn on LED
	 PORTB |= 0b00100000;
	_delay_ms(BLINK_MS);
	// Turn off LED
	PORTB &= ~0b00100000;
	_delay_ms(BLINK_MS);*/
	}
}

