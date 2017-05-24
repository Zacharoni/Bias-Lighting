// Really simple PWM test for 3 individual 1-color LEDs. 
// This is pretty irrelevant to the operation of the WS2812,
// but is still cool to see how PWM is done on AVR uCs.

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void pwm_init()
{
    // This line sets: Timer mode to CTC - Clock with no prescaling. pg 81
    TCCR0|=(1<<WGM00)|(1<<WGM01)|(1<<COM01)|(1<<CS00); 
    // This line sets: Clear OC1A/OC1B on compare match (set to low level) -
    TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);       
    // This line sets: Combined with previous register, Fast PWM. Top = OCR1A.
    // Clock with no prescaling. pg 111, 112
    TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS10); 
    ICR1=255; 
    // Setting color controls. Set blue to pin 18(PD4), green to 19(PD5), and red to 4(PB3)
    DDRB|=(1<<PB3);
    DDRD|=(1<<PD5)|(1<<PD4);
    // Output compare register. TOP of CTC is defined by OCR0.
    OCR0=0;
    // OCR1A is compared to TCNT1.
    OCR1A=0;
    // Defining third led
    OCR1B=0;
}

// Simple delay function
void wait()
{
    _delay_loop_2(3200);
}

void set_pwm(uint8_t duty1, uint8_t duty2, uint8_t duty3)
{
	OCR0 = duty1;
	OCR1A = duty2;
	OCR1B = duty3;
}

void main()
{
	//int red = 0, green = 123, blue = 255;
	uint8_t bright;
    pwm_init();
    // Output ports for the LEDs
	PORTB = 0xF7;
	PORTD = 0xCF;
    while(1){
		for (bright=0; bright<255; bright++){
        	set_pwm(bright, bright, bright);
			wait();
		}

		for (bright=255; bright>0; bright--){
			set_pwm(bright, bright, bright);
			wait();
		}
	}
} 
