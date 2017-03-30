#include "Motor.h"
#include "colorSensor.h"
#include "servo.h"

Motor leftMotor1(2, 3);
Motor leftMotor2(4, 5);
Motor rightMotor1(6, 7);
Motor rightMotor2(8, 9);
Motor strafeMotor(10, 11);
ColorSensor colour_sensor(44, 46, 48, 50, 52);
#define servo_pin 45
#define fire_sensor 47
void hDrive(double move, double rotate, double strafe) {
  double leftSpeed, rightSpeed;
  
  move = constrain(move, -1.0, 1.0);
  rotate = constrain(rotate, -1.0, 1.0);
  strafe = constrain(strafe, -1.0, 1.0);

  if (move > 0.0) {
    if (rotate > 0.0) {
      leftSpeed =  move - rotate;
      rightSpeed = max(move, rotate);
    } else {
      leftSpeed = max(move, -rotate);
      rightSpeed = move + rotate;
    }
  } else {
    if (rotate > 0.0) {
      leftSpeed = -max(-move, rotate);
      rightSpeed = move + rotate;
    } else {
      leftSpeed = move - rotate;
      rightSpeed = -max(-move, -rotate);
    }
  }

  leftMotor1.set(constrain(leftSpeed, -1.0, 1.0));
  leftMotor2.set(constrain(leftSpeed, -1.0, 1.0));
  rightMotor1.set(-constrain(rightSpeed, -1.0, 1.0));
  rightMotor2.set(-constrain(rightSpeed, -1.0, 1.0));
  strafeMotor.set(strafe);
}

void setup() {
  Serial.begin(9600);
  pinMode(servo_pin, OUTPUT);
  pinMode(fire_sensor, INPUT);
 }
 
void scanForFire(){
  servo_pin.write(0);
  for(int i = 0; i <= 180;i += 10){
    if(digitalRead(fire_sensor)){
      delay(10);//make sure that the reading isn't a false positive
      while(digitalRead(fire_sensor))
      {
        //Turn on extinguisher
        delay(50);
      }
    }
    break;
  }
}
int btn = 0;
void loop() {
  
  // put your main code here, to run repeatedly:
  if(digitalRead(21)){
    delay(100);
    Serial.println("pressed");
    
    if(!btn){
      //hDrive(1.0, 0.0, 0.0);
      delay(100);
      
      int ri = colour_sensor.getColor('r');
      int r = ri;
      int g = colour_sensor.getColor('g');
      int b = colour_sensor.getColor('b');
      String colour = String(ri) + " " + String(g) + " " + String(b);
      Serial.println(colour);
      hDrive(.3,0,0);
      while(r > ri - 30)
      {
        
        r = colour_sensor.getColor('r');
        String colour = String(r);
        Serial.println(colour);
      }
      Serial.println("Done");
      hDrive(0.0, 0.0, 0.0);
      btn = 1;
      
    }
    else{
      hDrive(0.0, 0.0, 0.0);
      btn = 0;
    }
  
  }
}
