/*	Author: Marco Baez
 *  Partner(s) Name: NA
 *	Lab Section:21
 *	Assignment: Lab #8  Exercise #2
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

enum States{Off, waitOff, On, waitOn, Inc, waitInc, Dec, waitDec}state;

unsigned char count = 0x00;

double frequency[8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25};

unsigned char button = 0x00;

void tick(){
	button = ~PINA & 0x07;
	switch(state){
		case Off:
			if(button==2){state=On;}
			else {state=Off;}
			break;

		case waitOff:
			if(!(button==2)){state=Off;}
			else{state=waitOff;}
			break;

		case On:
			if(button==2){state=On;}
			else{state=waitOn;}
			break;

		case waitOn:
			if(button==1){
				if(count<7){++count; state=Inc;}
			}
			else if(button==4){
				if(count>0){--count; state=Dec;}
			}
			else if(button==2){state=waitOff;}
			else{state=waitOn;}
			break;

		case Inc:
			state=waitInc;
			break;

		case waitInc:
			if(button==1){state=waitInc;}
			else {state=waitOn;}
			break;

		case Dec:
			state=waitDec;
			break;

		case waitDec:
			if(button==4){state=waitDec;}
			else {state=waitOn;}
			break;
	}	

	switch(state){
		case Off:
			PWM_off();
			break;

		case waitOff:
			break;

		case On:
			PWM_on();
			break;

		case waitOn:
			//set_PWM(293.66);
			break;

		case Inc:
			set_PWM(frequency[count]);
			break;

		case waitInc:
			break;

		case Dec:
			set_PWM(frequency[count]);
			break;

		case waitDec:
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
