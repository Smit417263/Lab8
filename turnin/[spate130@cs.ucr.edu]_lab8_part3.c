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
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif


 volatile unsigned char TimerFlag = 0;
  unsigned long _avr_timer_M = 1;
  unsigned long _avr_timer_cntcurr = 0;
  
  void TimerOn() {
	  TCCR1B = 0x0B;

	  OCR1A = 125;
	  TIMSK1 = 0x02;

	  TCNT1=0;

	  _avr_timer_cntcurr = _avr_timer_M;

	  SREG |= 0x80; // 0x80: 1000000
  }

  void TimerOff() {
	  TCCR1B = 0x00;
  }

  void TimerISR() {
	  TimerFlag = 1;
  }

  ISR(TIMER1_COMPA_vect) {
	  _avr_timer_cntcurr--;
	  if (_avr_timer_cntcurr == 0) {
		  TimerISR();
		  _avr_timer_cntcurr = _avr_timer_M;
	  }
  }

  void TimerSet(unsigned long M) {
	  _avr_timer_M = M;
	  _avr_timer_cntcurr = _avr_timer_M;
  }



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


enum states {on, play, off, down} state;

double sound[15] = {420.00, 463.88, 480.25, 460.63 , 420.00, 440.00, 333.43, 420.00, 392.00, 600.00, 500.00, 400.00, 300.00, 420.00, 463.88}; 
	
int i = 0;

unsigned char button;

int j = 0;  

void Tick(){
	switch(state) {
		case on:
			state = off;
			break;
		case off:
			if (button == 0x01) {
				i = 0; 
				j = 0;
				state = play;
			}
			break;
		case play:
			if (i == 30 ) {
				state = down;
			}
			break;
		case down:
			if (button == 0x00){
				state = off;
			}
			break;
		default:
			break;
	}
	switch(state){
		case on:
			break;
		case off:
			set_PWM(0.00);  //turn off
			break;
		case play:
			if (i == 0){
				set_PWM(sound[j]);
				j++;
			}
			else if (i== 1){
				set_PWM(sound[j]);
				j++;
			}
			else if (i== 2){
				set_PWM(sound[j]);
				j++;
			}
			else if (i== 4){
				set_PWM(sound[j]);
				j++;
			}
			else if (i== 6){
				set_PWM(sound[j]);
				j++;
			}
			else if (i == 8){
				set_PWM(sound[j]);
				j++;
			}
			else if (i == 10){
				set_PWM(sound[j]);
				j++;
			}
			else if (i == 12){
				set_PWM(sound[j]);
				j++;
			}
			else if (i == 14){
				set_PWM(sound[j]);
				j++;
			}
			else if (i == 16){
				set_PWM(sound[j]);
				j++;
			}
			else if (i == 18){
				set_PWM(sound[j]);
				j++;
			}
			else if (i == 20){
				set_PWM(sound[j]);
				j++;
			}
			else if (i == 22){
				set_PWM(sound[j]);
				j++;
			}
			else if (i == 24){
				set_PWM(sound[j]);
				j++;
			}
			else if (i == 26){
				set_PWM(sound[j]);
				j++;
			}
			else if (i == 28){
				set_PWM(sound[j]);
				j++;
			}
			
			i++;
			break;
		case down:
			set_PWM(0.00);
			break;
		default:
			break;
	}
}


int main() {

	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;

	PWM_on();
	TimerSet(200);
	TimerOn();
	state = on;

	while(1) {
		button = ~PINA;
		Tick();
		while (!TimerFlag){}  
		TimerFlag = 0; 

	}

	return 1;
}

