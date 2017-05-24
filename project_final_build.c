/*
* RGB LED Bias Lighting for TV or PC Monitor
* ATMega16 Microcontroller 
* WS2812 LED - Tested on 60 LED strip
* 8MHz Clock and Internal Oscillator Frequency
*
* Author: Zach Liebold
*
* WS2812 LED control library: (output_grb.s)
* Author: Mike Silva
* https://www.embeddedrelated.com/showarticle/528.php
*
* This program creates a bias lighting setup using a WS2812 LED strip. 
* It is controlled by a ATMega16 microcontroller. Using the STK500 board,
* a series of profiles are programmed that can be selected using the on board switches.
* End user may also switch between different colors and light patterns using the 
* selection switches to toggle through the settings.
*/

// Setting the clock frequency. Make sure to also set 
// the oscillator frequency to 8MHz in AVR Studio or LEDs will not work
#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;

// Define number of LEDs in the strip to be controlled
#define NUM_WS2812 60
#define NUM_LEDS (NUM_WS2812*3)

// Define number of states to cycle between
#define NUM_STATES 8

// Max value for brightness, on a scale of 0-255. 
// 50 is almost full brightness due to non-linear nature of LEDs.
// This also keeps the current draw low so more LEDs can be controlled.
#define MAX 50

// Declaration of the assembly function.
// This needs to be called each time a change in color is made.
extern void output_grb(u8 * ptr, u16 count);

// This function sets the brightness of each individual LED to control color
void set_color(u8 * p_buf, u8 led, u8 r, u8 g, u8 b)
{
  u16 index = 3*led;
  p_buf[index] = g;
  p_buf[index + 1] = r;
  p_buf[index + 2] = b;  
}

void initialize()
{
    // Initialize switches to Port D
    DDRD = 0x00;
    PORTD = 0xFF;
    DDRB = 0x01;        // Output set to PB0
}

// This will eventually change to having 1 profile for all colors, and 2 toggle switches
// to control color and breathing, as well as possibly breathing rate. 
// This can probably be done with a case loop, have a integer variable that increments based off of a button push
// and then select the correct case with proper colors. Modulo arithmetic to wrap around.
// For now we'll stick to one color to test algorithms and button functionality.

// For cycling, use the following colors:
// ROYGBIV
// O: r = MAX, g = 25, b = 0
// Y: r = MAX, g = MAX, b = 0
// I: r = 15, g = 0, b = 25
// V: r = 29, g = 0, b = 41

int main(void)
{
    int pos = 0, r = 0, g = 0, b = 0, i = 0;
    uint8_t state = 100;
    uint8_t delay = 10;         
    unsigned char x = 0xFF, x_old = 0xFF;
    u8 buf[NUM_LEDS];
    initialize();
    memset(buf, 0, sizeof(buf)); // Buffer the LEDs in memory

	while(1){
	    
	    x = PIND;
		if(x!=0xFF && x!=x_old){
			_delay_ms(10);
			x = PIND;
			if(x!=0xFF && x!=x_old){
				state++;
				if(state>=NUM_STATES) state = 0;
				x_old = x;
			}
		}
		
	    switch(state){
		
		// Red breathe
        case 0:
    		for (r=0; r<MAX; r++){
            	for(pos=0; pos<NUM_WS2812 + 1; pos++){
                	output_grb(buf, sizeof(buf));
                	set_color(buf, pos, r, 0, 0);
            	}
            	_delay_ms(delay);
        	}	
        	
        	for(r=MAX; r>0; r--){
            	for(pos=0; pos<NUM_WS2812 + 1; pos++){
                	output_grb(buf, sizeof(buf));
                	set_color(buf, pos, r, 0, 0);
            	}
            	_delay_ms(delay);
    		}
		break;
		
		// Yellow breathe
        case 1:
    		for (r=0; r<MAX; r++){
    			g = r;
            	for(pos=0; pos<NUM_WS2812 + 1; pos++){
                	output_grb(buf, sizeof(buf));
                	set_color(buf, pos, r, g, 0);
            	}
            	_delay_ms(delay);
        	}	
        	
        	for(r=MAX; r>0; r--){
        		g = r;
            	for(pos=0; pos<NUM_WS2812 + 1; pos++){
                	output_grb(buf, sizeof(buf));
                	set_color(buf, pos, r, g, 0);
            	}
            	_delay_ms(delay);
    		}
		break;
	    
	    // Green breathe
        case 2:
    		for (g=0; g<MAX; g++){
            	for(pos=0; pos<NUM_WS2812 + 1; pos++){
                	output_grb(buf, sizeof(buf));
                	set_color(buf, pos, 0, g, 0);
            	}
            	_delay_ms(delay);
        	}	
        	
        	for(g=MAX; g>0; g--){
            	for(pos=0; pos<NUM_WS2812 + 1; pos++){
                	output_grb(buf, sizeof(buf));
                	set_color(buf, pos, 0, g, 0);
            	}
            	_delay_ms(delay);
    		}
		break;
		
		// Sky blue breathe
        case 3:
    		for (g=0; g<MAX; g++){
    			b = g;
            	for(pos=0; pos<NUM_WS2812 + 1; pos++){
                	output_grb(buf, sizeof(buf));
                	set_color(buf, pos, 0, g, b);
            	}
            	_delay_ms(delay);
        	}	
        	
        	for(g=MAX; g>0; g--){
        		b = g;
            	for(pos=0; pos<NUM_WS2812 + 1; pos++){
                	output_grb(buf, sizeof(buf));
                	set_color(buf, pos, 0, g, b);
            	}
            	_delay_ms(delay);
    		}
		break;

		// Blue breathe
        case 4:
    		for (b=0; b<MAX; b++){
            	for(pos=0; pos<NUM_WS2812 + 1; pos++){
                	output_grb(buf, sizeof(buf));
                	set_color(buf, pos, 0, 0, b);
            	}
            	_delay_ms(delay);
        	}	
        	
        	for(b=MAX; b>0; b--){
            	for(pos=0; pos<NUM_WS2812 + 1; pos++){
                	output_grb(buf, sizeof(buf));
                	set_color(buf, pos, 0, 0, b);
            	}
            	_delay_ms(delay);
    		}
		break;
		
		// Purple breathe
        case 5:
    		for (r=0; r<MAX; r++){
    			b = r;
            	for(pos=0; pos<NUM_WS2812 + 1; pos++){
                	output_grb(buf, sizeof(buf));
                	set_color(buf, pos, r, 0, b);
            	}
            	_delay_ms(delay);
        	}	
        	
        	for(r=MAX; r>0; r--){
        		b = r;
            	for(pos=0; pos<NUM_WS2812 + 1; pos++){
                	output_grb(buf, sizeof(buf));
                	set_color(buf, pos, r, 0, b);
            	}
            	_delay_ms(delay);
    		}
		break;
		
		// White breathe
        case 6:
    		for (r=0; r<MAX; r++){
    			b = r;
    			g = r;
            	for(pos=0; pos<NUM_WS2812 + 1; pos++){
                	output_grb(buf, sizeof(buf));
                	set_color(buf, pos, r, g, b);
            	}
            	_delay_ms(delay);
        	}	
        	
        	for(r=MAX; r>0; r--){
        		b = r;
        		g = r;
            	for(pos=0; pos<NUM_WS2812 + 1; pos++){
                	output_grb(buf, sizeof(buf));
                	set_color(buf, pos, r, g, b);
            	}
            	_delay_ms(delay);
    		}
		break;

		// Rainbow
		case 7:
            r = MAX;
            g = 0;
            b = 0;
            
            // High red, cycle up green
            for(g=0; g<MAX; g++){
                output_grb(buf, sizeof(buf));
                set_color(buf, pos, r, g, b);
                pos = (pos + 1) % NUM_WS2812;
                _delay_ms(25);
            }

            // High red and green, cycle down red
            for(r=MAX; r>0; r--){
                output_grb(buf, sizeof(buf));
                set_color(buf, pos, r, g, b);
                pos = (pos + 1) % NUM_WS2812;
                _delay_ms(25);
            }

            // High green, cycle up blue
            for(b=0; b<MAX; b++){
                output_grb(buf, sizeof(buf));
                set_color(buf, pos, r, g, b);
                pos = (pos + 1) % NUM_WS2812;
                _delay_ms(25);
            }

            // High green and blue, cycle down green
            for(g=MAX; g>0; g--){
                output_grb(buf, sizeof(buf));
                set_color(buf, pos, r, g, b);
                pos = (pos + 1) % NUM_WS2812;
                _delay_ms(25);
            }

            //High blue, cycle up red
            for(r=0; r<MAX; r++){
                output_grb(buf, sizeof(buf));
                set_color(buf, pos, r, g, b);
                pos = (pos + 1) % NUM_WS2812;
                _delay_ms(25);
            }

            //High red and blue, cycle down blue
            for(b=MAX; b>0; b--){
                output_grb(buf, sizeof(buf));
                set_color(buf, pos, r, g, b);
                pos = (pos + 1) % NUM_WS2812;
                _delay_ms(25);
            }
        break;

		default:
			r = 0;
			g = 0;
			b = 0;
			for(pos=0; pos<NUM_WS2812 + 1; pos++){
				output_grb(buf, sizeof(buf));
				set_color(buf, pos, r, g, b);
			}
		break;

	    }
	}
}
