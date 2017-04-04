#ifndef Motor_h
#define Motor_h

#include "Arduino.h"

class Motor {
	public:
//		Motor(int in1, int in2, int en);
    Motor(int in1, int in2);
		void set(double speed);
	private:
		int _in1;
		int _in2;
		int _en;
};

#endif
