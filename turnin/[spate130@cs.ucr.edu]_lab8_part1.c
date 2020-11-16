/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #8  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link:  https://youtu.be/pbix6ZRrSFk
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency) {
	static double current_frequency; 
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; } 
		else { TCCR3B |= 0x03; } 
		
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		
		else if (frequency > 31250) { OCR3A = 0x0000; }
		
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

		TCNT0 = 0; 
		current_frequency = frequency; 
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	unsigned char button = 0;
	PWM_on();

	double c4 =261.63;
	double d4 =293.66;
	double e4 =329.63;

    /* Insert your solution below */
    while (1) {

	button = ~PINA & 0x07;

	if(button == 0x01){
		set_PWM(c4);
	}
	else if (button == 0x02){
		set_PWM(d4);
	}
	else if(button == 0x04){
		set_PWM(e4);
	}
	else{
		set_PWM(0);
	}

    }
    return 1;
}
