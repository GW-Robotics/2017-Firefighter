#ifndef Ultrasonic_h
#define Ultrasonic_h

#include "Arduino.h"

class Ultrasonic {
	private:
		int echo, trig, conversionFactor;
		
	public:
		Ultrasonic(int echoPin, int trigPin, bool useInches);
		float getDistance();
};

#endif
