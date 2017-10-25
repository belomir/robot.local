/*
 * Wheelbot.cpp
 * © Roganov Sergey, Novosibirsk, 2016
 * GPL v3+
 */

#include "Arduino.h"
#include "Wheelbot.h"

Wheelbot::Wheelbot(int m1d, int m1s, int m2s, int m2d)
{
	p_m1_d = m1d;
	p_m1_s = m1s;
	p_m2_s = m2s;
	p_m2_d = m2d;
}

void Wheelbot::Stop()
{
	digitalWrite(p_m1_s, LOW);
	digitalWrite(p_m2_s, LOW);
}

void Wheelbot::Forward()
{
	digitalWrite(p_m1_d, LOW);
	digitalWrite(p_m2_d, LOW);
	digitalWrite(p_m1_s, HIGH);
	digitalWrite(p_m2_s, HIGH);
}

void Wheelbot::Forward(unsigned char speed)
{
	digitalWrite(p_m1_d, LOW);
	digitalWrite(p_m2_d, LOW);
	digitalWrite(p_m1_s, speed);
	digitalWrite(p_m2_s, speed);
}

void Wheelbot::Backward()
{
	digitalWrite(p_m1_d, HIGH);
	digitalWrite(p_m2_d, HIGH);
	digitalWrite(p_m1_s, HIGH);
	digitalWrite(p_m2_s, HIGH);
}

void Wheelbot::Backward(unsigned char speed)
{
	digitalWrite(p_m1_d, HIGH);
	digitalWrite(p_m2_d, HIGH);
	digitalWrite(p_m1_s, speed);
	digitalWrite(p_m2_s, speed);
}

void Wheelbot::Left()
{
	digitalWrite(p_m1_d, HIGH);
	digitalWrite(p_m2_d, LOW);
	digitalWrite(p_m1_s, HIGH);
	digitalWrite(p_m2_s, HIGH);
}

void Wheelbot::Left(unsigned char speed)
{
	digitalWrite(p_m1_d, HIGH);
	digitalWrite(p_m2_d, LOW);
	digitalWrite(p_m1_s, speed);
	digitalWrite(p_m2_s, speed);
}

void Wheelbot::Right()
{
	digitalWrite(p_m1_d, LOW);
	digitalWrite(p_m2_d, HIGH);
	digitalWrite(p_m1_s, HIGH);
	digitalWrite(p_m2_s, HIGH);
}

void Wheelbot::Right(unsigned char speed)
{
	digitalWrite(p_m1_d, LOW);
	digitalWrite(p_m2_d, HIGH);
	digitalWrite(p_m1_s, speed);
	digitalWrite(p_m2_s, speed);
}

void Wheelbot::ForwardRandomly(unsigned char delta)
{
	digitalWrite(p_m1_d, LOW);
	digitalWrite(p_m2_d, LOW);
	digitalWrite(p_m1_s, HIGH);
	digitalWrite(p_m2_s, HIGH);
	unsigned char p = random(100);
	unsigned char v = random(250-delta, 250);
	if(p > 50){
		analogWrite(p_m1_s, v);
	}else{
		analogWrite(p_m2_s, v);
	}
}
