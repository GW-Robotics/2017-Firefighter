#include "Arduino.h"
#include "Motor.h"

//Motor::Motor(int in1, int in2, int en) {
//	_in1 = in1;
//	_in2 = in2;
//	_en = en;
//	
//	pinMode(_in1, OUTPUT);
//	pinMode(_in2, OUTPUT);
//	pinMode(_en, OUTPUT);
//}
//
//void Motor::set(double speed) {
//	analogWrite(_en, (255 * abs(speed)));
//	
//	if (speed > 0.0) {
//		digitalWrite(_in1, HIGH);
//		digitalWrite(_in2, LOW);
//	} else if (speed < 0.0) {
//		digitalWrite(_in1, LOW);
//		digitalWrite(_in2, HIGH);
//	} else {
//		digitalWrite(_in1, LOW);
//		digitalWrite(_in2, LOW);
//	}
//}

Motor::Motor(int in1, int in2) {
  _in1 = in1;
  _in2 = in2;
  
  pinMode(_in1, OUTPUT);
  pinMode(_in2, OUTPUT);
}

void Motor::set(double speed) {
  if (speed > 0.0) {
    analogWrite(_in1, abs(speed) * 255);
    analogWrite(_in2, 0);
  } else if (speed < 0.0) {
    analogWrite(_in1, 0);
    analogWrite(_in2, abs(speed) * 255);
  } else {
    analogWrite(_in1, 0);
    analogWrite(_in2, 0);
  }
}
