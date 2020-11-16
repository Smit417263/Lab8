/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section: 023
 *	Assignment: Lab #8  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://youtu.be/pbix6ZRrSFk
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


enum States{off, off1, on, inc, up, wait1, down, wait2}state;


unsigned char button = 0x00;

const double sound[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};


unsigned char i = 0x00;

void Tick(){
	button = ~PINA & 0x07;
	switch(state){ // Transitions
		case off:
			if(button == 1)
				state = on;
			else
				state = off;
			break;
		case off1:
			if(!(button == 1))
				state = off;
			else
				state = off1;
			break;
		case on:
			if(button == 1)
				state = on;
			else
				state = inc;
			break;
		case inc:
			if(button == 2){
				if(i < 7)
					i++;
				state = up;
			}
			else if(button == 4){
				if(i > 0)
					i--;
				state = down;
			}
			else if(button == 1)
				state = turnOff;
			else
				state = inc;
			break;
		case up:
			state = wait1;
			break;
		case wait1:
			if(button == 2)
				state = wait1;
			else
				state = inc;
			break;
		case down:
			state = wait2;
			break;
		case wait2:
			if(button == 4)
				state = wait2;
			else
				state = inc;
			break;
	}
	switch(state){ // State actions
		case off:
			PWM_off();
			break;
		case turnOff:
			break;
		case on:
			PWM_on();
			break;
		case inc:
			break;
		case up:
			set_PWM(sound[i]);
			break;
		case wait1:
			break;
		case down:
			set_PWM(sound[i]);
			break;
		case wait2:
			break;

	}
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	PWM_on();
	state = off;
   /* Insert your solution below */
    while (1) {
	Tick();
    }
    return 1;
}
