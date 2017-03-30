#include "Motor.h"
#include "Ultrasonic.h"
#include "colorSensor.h"
#include "servo.h"

#define WALL_DISTANCE 5

#define servo_pin 45
#define fire_sensor 47

Motor leftMotor1(2, 3);
Motor leftMotor2(4, 5);
Motor rightMotor1(6, 7);
Motor rightMotor2(8, 9);
Motor strafeMotor(10, 11);

ColorSensor colour_sensor(44, 46, 48, 50, 52);

Ultrasonic frontUltrasonic(22, 23, true);
Ultrasonic leftUltrasonic(24, 25, true);
Ultrasonic rightUltrasonic(26, 27, true);
Ultrasonic backUltrasonic(28, 29, true);

// Drivetrain commands
void driveToWhite();
void driveThroughRoom();
void hDrive(double move, double rotate, double strafe);
void definedStartSearch();

// Extinguisher commands
void scanForFire();

void setup() {
  Serial.begin(9600);
  pinMode(servo_pin, OUTPUT);
  pinMode(fire_sensor, INPUT);
}
 
int btn = 0;

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(21)){
    delay(100);
    
    if(!btn){
      delay(100);
      
      driveToWhite()
    }
    else{
      hDrive(0.0, 0.0, 0.0);
    }

    btn = !btn;
  }
}

void driveToWhite() {
  int ri = colour_sensor.getColor('r');
  int r = ri;
  int g = colour_sensor.getColor('g');
  int b = colour_sensor.getColor('b');
//      String colour = String(ri) + " " + String(g) + " " + String(b);
//      Serial.println(colour);
  hDrive(.3,0,0);
  
  while(r > ri - 30) { 
    r = colour_sensor.getColor('r');
//        String colour = String(r);
//        Serial.println(colour);
  }
  
//      Serial.println("Done");
  hDrive(0.0, 0.0, 0.0);
}

void driveThroughRoom() {
  if (frontUltrasonic.get_distance() < WALL_DISTANCE) {
    if (leftUltrasonic.get_distance() < WALL_DISTANCE) {
      hDrive(0.0, 0.0, 1.0);
    } else {
      hDrive(0.0, 0.0, -1.0);
    }
  } else {
    if (leftUltrasonic.get_distance() < WALL_DISTANCE) {
      hDrive(0.0, 0.0, 1.0);
    } else {
      hDrive(0.0, 0.0, -1.0);
    }
  }
}

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

void definedStartSearch() {
  // Start A
  if (leftUltrasonic.get_distance() < 10) {
    hDrive(1.0, 0.0, 0.0);
    
    while (leftUltrasonic.get_distance() < 7) { }
    hDrive(0.0, 1.0, 0.0);
    
  // Start B
  } else {
    // Strafe to Room4
    hDrive(0.0, 0.0, -1.0);

    while (frontUltrasonic().get_distance() > 7) { }
    while (frontUltrasonic().get_distance() < 7) { }

    hDrive(0.0, 0.0, 0.0);
    delay(100);
    driveToWhite();
  }
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

