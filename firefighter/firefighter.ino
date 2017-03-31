#include "Ultrasonic.h"
#include "ColorSensor.h"
#include "Motor.h"
#include <Servo.h>

#define FLAME_LED 53
#define SOUND_LED 52

#define WALL_DISTANCE 7

#define flame_sensor 30
#define servo_pin 31
#define interval 10

Motor fan_motor(50,51);
Servo extingusher_servo;

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

// Sound Sensor
int sensorPin = A0;   
int sensorValue = 0;
bool robotOn = false;
bool soundStart();

// Extinguisher
void extinguish();

void setup() {
  Serial.begin(9600);
  
  // Status Lights
  pinMode(SOUND_LED, OUTPUT);
  pinMode(FLAME_LED, OUTPUT);

  // Right Encoder
  directionRight = true;//default -> Forward  
  pinMode(encoderBRight,INPUT);  
  attachInterrupt(5, countPulseRight, CHANGE);

  // Left Encoder
  directionLeft = true;//default -> Forward  
  pinMode(encoderBLeft,INPUT);  
  attachInterrupt(4, countPulseLeft, CHANGE);
  
  // Sound Sensor
  pinMode(sensorPin, INPUT);

  // Extinguisher
  pinMode(flame_sensor, INPUT);

  extingusher_servo.attach(servo_pin);
  extingusher_servo.write(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (soundStart() || digitalRead(13)) {
    digitalWrite(SOUND_LED, HIGH);
    robotOn = !robotOn;
  }

  delay(300);

  if(robotOn){
//    Serial.println(pulseRight);
//    definedStartSearch();
//	  digitalWrite(FLAME_LED, HIGH);
//	  delay(1000);
//	  
//	  digitalWrite(FLAME_LED, LOW);
//	  delay(1000);
    extinguish();
  }
  else{
    hDrive(0.0, 0.0, 0.0);
  }

  delay(100);
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

bool soundStart(){
  return (analogRead(sensorPin) > 500);
}

void drivetrainCheck() {
  hDrive(0.3, 0.0, 0.0); delay(300); hDrive(-0.3, 0.0, 0.0); delay(300);
  hDrive(0.0, 0.3, 0.0); delay(300); hDrive(0.0, -0.3, 0.0); delay(300);
  hDrive(0.0, 0.0, 0.3); delay(300); hDrive(0.0, 0.0, -0.3); delay(300);
}

void ultrasonicCheck() {
	Serial.println("====================================");
	Serial.print("F: ");
	Serial.println(frontUltrasonic.getDistance());
	Serial.print("L: ");
	Serial.println(leftUltrasonic.getDistance());
	Serial.print("R: ");
	Serial.println(rightUltrasonic.getDistance());
	Serial.print("B: ");
	Serial.println(backUltrasonic.getDistance());
	delay(300);
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

void countPulseRight() {
  int Lstate = digitalRead(encoderARight);
  if((encoderALastRight == LOW) && Lstate==HIGH)
  {
    int val = digitalRead(encoderBRight);
    if(val == LOW && directionRight)
    {
      directionRight = false; //Reverse
    } else if(val == HIGH && !directionRight) {
      directionRight = true;
    }
  }
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
    } else if(val == HIGH && !directionLeft) {
      directionLeft = true;
    }
  }
  encoderALastLeft = Lstate;
 
  if(!directionLeft)  pulseLeft++;
  else  pulseLeft--;
}

double getAngle() {
  return ((pulseLeft - pulseRight) * 12.5 / 700) / 8;
}

double getPercentError(double actual, double target) {
  return abs(actual - target) / target;
}

void turnToAngle(double targetAngle) {
  while (abs(getAngle()) < abs(targetAngle)) {
    hDrive(0.0, getPercentError(getAngle(), targetAngle), 0.0);
  }
  hDrive(0.0, 0.0, 0.0);
}

void extinguish() {
  for(int i = 0; i <= 180; i += interval){
    extingusher_servo.write(i);
    delay(15*interval);//give time for servo to move

    if(!digitalRead(flame_sensor))
      digitalWrite(FLAME_LED, HIGH);
  
    //turn on fan
    fan_motor.set(1);
    while(!digitalRead(flame_sensor)){
      //while there is a fire
      delay(1000);   
    }
    //turn off fan
    fan_motor.set(0);
    digitalWrite(FLAME_LED, LOW);  
  }
  
  for(int i = 180; i >=0; i -= interval){
    extingusher_servo.write(i);
    delay(15*interval);//give time for servo to move

    if(!digitalRead(flame_sensor))
      digitalWrite(FLAME_LED, HIGH);
  
    //turn on fan
    fan_motor.set(1);
    while(!digitalRead(flame_sensor)){
      //while there is a fire
      delay(1000);   
    }
    //turn off fan
    fan_motor.set(0);
    digitalWrite(FLAME_LED, LOW);  
  }
}

