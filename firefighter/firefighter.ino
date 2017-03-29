#include "Motor.h"

Motor leftMotor1(2, 3, 4);
Motor leftMotor2(5, 6, 7);
Motor rightMotor1(8, 9, 10);
Motor rightMotor2(11, 12 , 13);
Motor strafeMotor(40, 42, 44);

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
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
  hDrive(1.0, 0.0, 0.0);
}
