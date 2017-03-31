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
    else if(val == HIGH && !directionRight)
    {
      directionRight = true;  //Forward
    }
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
    else if(val == HIGH && !directionLeft)
    {
      directionLeft = true;  //Forward
    }
  }
}

double getAngle() {
  return ((pulseLeft - pulseRight) * 12.5 / 700) / 8;
}

double getPercentError(double actual, double target) {
  return abs(actual - target) / target;
}

void turnToAngle(double targetAngle) {
  while (abs(getAngle()) < abs(targetAngle)) {
    hDrive(0.0, getPercentError(getAngle(), targetAngle()), 0.0);
  }
  hDrive(0.0, 0.0, 0.0);
}


#include "Ultrasonic.h"
#include "ColorSensor.h"
#include <Servo.h>

#define WALL_DISTANCE 5


Motor leftMotor1(3, 4);
Motor leftMotor2(5, 6);
Motor rightMotor1(7, 8);
Motor rightMotor2(9, 10);
Motor strafeMotor(11, 12);

ColorSensor colour_sensor(44, 46, 48, 50, 52);

Ultrasonic frontUltrasonic(28, 29, true);
Ultrasonic leftUltrasonic(26, 27, true);
Ultrasonic rightUltrasonic(24, 25, true);
Ultrasonic backUltrasonic(22, 23, true);

// Drivetrain commands
void driveToWhite();
void driveThroughRoom();
void hDrive(double move, double rotate, double strafe);
void definedStartSearch();
void turnToAngle();
double getAngle(double targetAngle);
double getPercentError(double actual, double target);
void turnToAngle(double targetAngle);

// Extinguisher commands
void scanForFire();

// Encoder commands
void countPulseRight();
int pulseRight;
const byte encoderARight = 18;
const byte encoderBRight = 16;
byte encoderALastRight;
bool directionRight;

int pulseLeft;
void countPulseLeft();
const byte encoderALeft = 19;
const byte encoderBLeft = 17;
byte encoderALastLeft;
bool directionLeft;

double getAngle();

void setup() {
  Serial.begin(9600);
  pinMode(13, INPUT);
  pinMode(servo_pin, OUTPUT);
  pinMode(fire_sensor, INPUT);

  directionRight = true;//default -> Forward  
  pinMode(encoderBRight,INPUT);  
  attachInterrupt(5, countPulseRight, CHANGE);

  directionLeft = true;//default -> Forward  
  pinMode(encoderBLeft,INPUT);  
  attachInterrupt(4, countPulseLeft, CHANGE);
}
 
bool btn = false;

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(13)) {
    btn = !btn;
  }

  if(btn){
    Serial.println(pulseRight);
  //      driveToWhite()
    definedStartSearch();
  }
  else{
    hDrive(0.0, 0.0, 0.0);
  }

  delay(100);
}

void driveToWhite() {
  int ri = colour_sensor.getColor('r');
  int r = ri;
//  int g = colour_sensor.getColor('g');
//  int b = colour_sensor.getColor('b');
//  String colour = String(ri) + " " + String(g) + " " + String(b);
//  Serial.println(colour);
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
  if (frontUltrasonic.getDistance() < WALL_DISTANCE) {
    if (leftUltrasonic.getDistance() < WALL_DISTANCE) {
      hDrive(0.0, 0.0, 1.0);
    } else {
      hDrive(0.0, 0.0, -1.0);
    }
  } else {
    if (leftUltrasonic.getDistance() < WALL_DISTANCE) {
      hDrive(0.0, 0.0, 1.0);
    } else {
      hDrive(0.0, 0.0, -1.0);
    }
  }
}

void definedStartSearch() {
  // Start A
  if (rightUltrasonic.getDistance() < WALL_DISTANCE) {
    hDrive(0.0, 0.0, -1.0);
    delay(500);
    while (frontUltrasonic.getDistance() < WALL_DISTANCE) { }
    hDrive(0.0, 0.0, 0.0);
  // Start B
  } else {
    hDrive(1.0, 0.0, 0.0);
    while (frontUltrasonic.getDistance() > WALL_DISTANCE) { }
    hDrive(0.0, 0.0, 0.0);
  }
}

void scanForFire(){
//  servo_pin.write(0);
//  for(int i = 0; i <= 180;i += 10){
//    if(digitalRead(fire_sensor)){
//      delay(10);//make sure that the reading isn't a false positive
//      while(digitalRead(fire_sensor))
//      {
//        //Turn on extinguisher
//        delay(50);
//      }
//    }
//    break;
//  }
}

void countPulseRight() {
  int Lstate = digitalRead(encoderARight);
  if((encoderALastRight == LOW) && Lstate==HIGH)
  {
    int val = digitalRead(encoderBRight);
    if(val == LOW && directionRight)
    {
      directionRight = false; //Reverse
  encoderALastRight = Lstate;
 
  if(!directionRight)  pulseRight++;
  else  pulseRight--;
}

void countPulseLeft() {
  int Lstate = digitalRead(encoderALeft);
  if((encoderALastLeft == LOW) && Lstate==HIGH)
  {
    int val = digitalRead(encoderBLeft);
    if(val == LOW && directionLeft)
    {
      directionLeft = false; //Reverse
  encoderALastLeft = Lstate;
 
  if(!directionLeft)  pulseLeft++;
  else  pulseLeft--;
}