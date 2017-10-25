/*
 * Wheelbot.h
 * © Roganov Sergey, Novosibirsk, 2016
 * GPL v3+
 */

#ifndef Wheelbot_h
#define Wheelbot_h

#include "Arduino.h"

class Wheelbot
{
	public:
		Wheelbot(int m1d, int m1s, int m2s, int m2d);
		void Stop();
		void Forward();
		void Forward(unsigned char speed);
		void Backward();
		void Backward(unsigned char speed);
		void Left();
		void Left(unsigned char speed);
		void Right();
		void Right(unsigned char speed);
		void ForwardRandomly(unsigned char delta);
	private:
		int p_m1_d;
		int p_m1_s;
		int p_m2_s;
		int p_m2_d;
};

#endif
