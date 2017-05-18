#ifndef ColorSensor_h
#define ColorSensor_h

#include "Arduino.h"

class ColorSensor {
  private:
    int s0, s1, s2, s3, sensorOut;
  
  public:
    ColorSensor(int s0, int s1, int s2, int s3, int sensorOut);
    int getColor(char color);
};

#endif
