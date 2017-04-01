#include "Ultrasonic.h"
#include "ColorSensor.h"
#include "Motor.h"
#include "FreqCount.h"
#include <Servo.h>

#define FLAME_LED 53
#define SOUND_LED 42

#define WALL_DISTANCE 7

#define flame_sensor 30
#define servo_pin 31
#define interval 10

#define LOW_START 3230
#define HIGH_START 4370

Servo extingusher_servo;

Motor leftMotor1(2, 3);
Motor leftMotor2(4, 5);
Motor rightMotor1(6, 7);
Motor rightMotor2(8, 9);
Motor strafeMotor(10, 11);
Motor fan_motor(12, 13);

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
double getAngle(double targetAngle);
double getPercentError(double actual, double target);
void turnToAngle(double targetAngle, double speed);
void naviguessMaze(double swagSpeed);

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
double getDistanceLeft();
double getDistanceRight();
void resetEncoders();

// Sound Sensor
bool robotOn = false;
unsigned long count;

// Extinguisher
void extinguish();

int startSwitch = 14;

void setup() {
  Serial.begin(9600);
  
  // Start Switch
  pinMode(startSwitch, INPUT);
  
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

  // Extinguisher
  pinMode(flame_sensor, INPUT);

  extingusher_servo.attach(servo_pin);
  extingusher_servo.write(90);

  FreqCount.begin(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(count);
  if (FreqCount.available()) {
    count = FreqCount.read();
  } else {
    count = 0;
  }
  
  if ((count > 3230 && count < 4370) || digitalRead(startSwitch)) {
    digitalWrite(SOUND_LED, HIGH);
    robotOn = !robotOn;
  }

  if(robotOn){
    FreqCount.end();
    resetEncoders();
//    extinguish();
    naviguessMaze(0.5);

  }
  else{
    digitalWrite(SOUND_LED, LOW);
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
 
  if(!directionRight)  pulseRight--;
  else  pulseRight++;
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
  return (180 / PI) * (getDistanceLeft() - getDistanceRight()) / 8;
}

double getDistanceLeft() {
	return (pulseLeft * 12.5) / 1356;
}

double getDistanceRight() {
	return (pulseRight * 12.5) / 1356;
}

void resetEncoders() {
	pulseLeft = pulseRight = 0;
}

double getPercentError(double actual, double target) {
  return abs(actual - target) / target;
}

void turnToAngle(double targetAngle, double speed) {
  while (abs(getAngle()) < abs(targetAngle)) {
    hDrive(0.0, speed, 0.0);
  }
  hDrive(0.0, 0.0, 0.0);
}

void extinguish() {
  for(int i = 40; i <= 160; i += interval){
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
  
  for(int i = 140; i >=20; i -= interval){
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

void naviguessMaze(double swagSpeed) {
	bool frontTriggered = false;
	bool leftTriggered = false;
	bool rightTriggered = false;
	bool sweetSpot =  false;
	
	if (frontUltrasonic.getDistance() < 10) {
		frontTriggered = true;
	} else if ((leftUltrasonic.getDistance() > 4) && (rightUltrasonic.getDistance() > 4)) {
		sweetSpot = true;
	} else if (leftUltrasonic.getDistance() < 4) {
		leftTriggered = true;
		frontTriggered = frontUltrasonic.getDistance() < 10;
	} else if (rightUltrasonic.getDistance() < 4) {
		rightTriggered = true;
		frontTriggered = frontUltrasonic.getDistance() < 10;
	} else {
		frontTriggered = leftTriggered = rightTriggered = sweetSpot = false;
	}
	
	if (sweetSpot) {
		hDrive(swagSpeed, 0.0, 0.0);
	} else if (leftTriggered && frontTriggered) {
		hDrive(0.0, swagSpeed , 0.0);
	} else if (rightTriggered && frontTriggered) {
		hDrive(0.0, -swagSpeed , 0.0);
	} else if (leftTriggered) {
		hDrive(swagSpeed - 0.2, swagSpeed, 0.0);
	} else if (rightTriggered) {
		hDrive(0.8, -swagSpeed , 0.0);
	} else {
		hDrive(swagSpeed, swagSpeed , 0.0);
	}
	
}
