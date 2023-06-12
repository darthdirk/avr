#include <stdio.h>
#include <stdint.h>



#define REFMVOLTS 3300
#define THRESHOLD_MV 1000
#define ADC_RESOLUTION_BITS 12 //bits


void ADC_config() {
}

int16_t ADC_read() {
}

int32_t ADC_counts_to_mV(int16_t counts){
}


int main(void)
{
	float volts;
	int32_t ADC_value=0;
	
	// set C0 to input mode
	DDRC &= ~(PORTC0);
		
}

