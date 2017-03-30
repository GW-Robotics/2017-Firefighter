#include "Motor.h"
#include "Ultrasonic.h"

Motor leftMotor1(12, 13);
Motor leftMotor2(4, 5);
Motor rightMotor1(6, 7);
Motor rightMotor2(8, 9);
Motor strafeMotor(10, 11);

Ultrasonic frontUltrasonic(22, 23, true);
Ultrasonic leftUltrasonic(24, 25, true);
Ultrasonic rightUltrasonic(26, 27, true);
Ultrasonic backUltrasonic(28, 29, true);

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
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //hDrive(1.0, 0.0, 0.0);
  Serial.println(1.0);
  delay(400);
}

