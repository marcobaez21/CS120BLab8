/*	Author: Marco Baez
 *  Partner(s) Name: NA
 *	Lab Section:21
 *	Assignment: Lab #8  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo link: https://drive.google.com/drive/folders/1-1_Zf0EL01XEZT5VoUOW-SXjQH8O5fE9?usp=sharing
 *
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void set_PWM(double frequency){
	static double current_frequency;

	if(frequency != current_frequency){
		if(!frequency){TCCR3B &= 0x08;}
		else {TCCR3B |= 0x03;}

		if(frequency<0.954){OCR3A = 0xFFFF;}

		else if(frequency>31250){OCR3A = 0x0000;}

		else{OCR3A = (short)(8000000/(128*frequency))-1;}

		TCNT3 = 0;
		current_frequency = frequency;
	}
}

void PWM_on(){
	TCCR3A = (1 << COM3A0);

	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);

	set_PWM(0);
}

void PWM_off(){
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}

enum States{Off, noteC, noteD, noteE}state;

unsigned char button = 0x00;

void tick(){
	button = ~PINA & 0x07;
	switch(state){
		case Off:
			if(button==1){state=noteC;}
			else if(button==2){state=noteD;}
			else if(button==4){state=noteE;}
			else{state=Off;}
			break;

		case noteC:
			if(button==1){state=noteC;}
			else{state=Off;}
			break;

		case noteD:
			if(button==2){state=noteD;}
			else{state=Off;}
			break;

		case noteE:
			if(button==4){state=noteE;}
			else{state=Off;}
			break;
	}	

	switch(state){
		case Off:
			set_PWM(0);
			break;

		case noteC:
			set_PWM(261.63);
			break;

		case noteD:
			set_PWM(293.66);
			break;

		case noteE:
			set_PWM(329.63);
			break;
	}
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    PWM_on();
    state=Off;
    while (1) {
	tick();
    }
    return 0;
}
