
#include <util/delay.h>
#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define PRESCALER 10 //10 bits ... 1024

#define CLOCK_RATE 15998976

//roughly the number of timer ticks in a second
#define COUNTER_VALUE (CLOCK_RATE >> PRESCALER)


//TIMER1 compare interrupt service routine
//tells the compiler this is an ISR and that its specifically
// when TIMER1s comparison hardware matches the value we specified  
//fires when counter value matches OCR1A   
ISR(TIMER1_COMPA_vect){
	PORTB ^= _BV(PORTB5);
}



int main(){
	
	//Put the B5 in output mode
	DDRB |= _BV(PORTB5);
	
	//OCR1A = output compare register for timer 1 
	//COUNTER_VALUE = the number of clock ticks that 
	//I want to wait until getting an interrupt
	OCR1A= COUNTER_VALUE;
	
	//TCCR1A timer counter configuration register 1
	//TCCR1B timer counter configuration register 2
	//CTC mode at each clock tick its going to compare the
	// timer of the counter to the OCR1A register
	// if it matches its going to clear the counter and give us an interrupt
	
	TCCR1A = 0x00;
	TCCR1B |= (1 << WGM12);; //CTC mode -- compare to OCR1A
	TCCR1B |= (1<<CS10) | (1<<CS12); //set the prescaler value of 1024
	// meaning that it will divide the clock by 1024
	
	
	//I want an interrupt
	TIMSK1 |= (1 << OCIE1A);
	
	//enable interrupts
	sei();
	
	while (true){
		//do something useful in here while we are waiting for the timer to fire
	}
	{
		
	}

}

