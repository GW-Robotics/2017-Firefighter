
#include "Ultrasonic.h"
#include "ColorSensor.h"
#include "Motor.h"
#include "FreqCount.h"
#include <Servo.h>

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

#define SOUND_LED 45
#define FLAME_LED 53

#define WALL_DISTANCE 7

#define flame_sensor 30
// #define servo_pin 31
// #define interval 10

#define LOW_START 3230
#define HIGH_START 4370

#define LEFT_MOTOR_1A 44
#define LEFT_MOTOR_1B 3

#define INTERRUPT_PIN 2

// PID
#define setpoint 0
#define kP 0.002

Servo extingusher_servo;

Motor leftMotor1(LEFT_MOTOR_1A, LEFT_MOTOR_1B);
Motor leftMotor2(4, 5);
Motor rightMotor1(6, 7);
Motor rightMotor2(8, 9);
Motor strafeMotor(10, 11);
Motor fan_motor(12, 13);

ColorSensor colour_sensor(42, 46, 48, 50, 52);

Ultrasonic frontUltrasonic(28, 29, true);
Ultrasonic leftUltrasonic(26, 27, true);
Ultrasonic rightUltrasonic(24, 25, true);
Ultrasonic backUltrasonic(22, 23, true);

//top ultrasonic array
Ultrasonic leftArrayInner(34, 35, true);//blue
Ultrasonic leftArrayOutter(32, 33, true);//orange

Ultrasonic rightArrayInner(36, 37, true);//orange
Ultrasonic rightArrayOutter(38, 39, true);//black

void ultrasonicArrayCheck();

MPU6050 mpu; // Default address 0x68

// Drivetrain commands
void driveToWhite();
void driveThroughRoom();
void hDrive(double move, double rotate, double strafe);
void pidMove(double move, double rotate, double strafe);
void tankDrive(double leftSpeed, double rightSpeed, double strafeSpeed);
void definedStartSearch();
double getPercentError(double actual, double target);
void turnToAngle(double targetAngle, double speed);
void naviguessMaze(double swagSpeed);
void driveToDistance(double targetDistance, double speed);

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

// Gyro
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
float euler[3];         // [psi, theta, phi]    Euler angle container

double gyroResetValX;
double gyroResetValY;
double gyroResetValZ;
void resetGyro(char axis);
double getAngle(char axis);

int ri;

bool pidEnabled = false;

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

void setup() {
  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
      Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
  #endif
    
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
  
  ri = -1;

  // extingusher_servo.attach(31);
  // extingusher_servo.write(90);

  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);
  
  devStatus = mpu.dmpInitialize();

  // accel offsets
  mpu.setXAccelOffset(-4265);
  mpu.setYAccelOffset(-283);
  mpu.setZAccelOffset(1675); // 1688 factory default for my test chip
  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(-51);
  mpu.setYGyroOffset(40);
  mpu.setZGyroOffset(27);

  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
      // turn on the DMP, now that it's ready
      mpu.setDMPEnabled(true);

      // enable Arduino interrupt detection
      attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
      mpuIntStatus = mpu.getIntStatus();

      // set our DMP Ready flag so the main loop() function knows it's okay to use it
      Serial.println(F("DMP ready! Waiting for first interrupt..."));
      dmpReady = true;

      // get expected DMP packet size for later comparison
      packetSize = mpu.dmpGetFIFOPacketSize();
  }

  resetGyro('z');

  hDrive(0.0, 0.0, 0.0);
  fan_motor.set(0.0);
  
  FreqCount.begin(1000);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  
}

bool firstStart = false;
bool foundFlame = false;

double heading = 0;

void loop() {
  // put your main code here, to run repeatedly:
  if (FreqCount.available()) {
    count = FreqCount.read();
	  //Serial.println(count);
  } else {
    count = 0;
  }
  
//  Serial.println(getAngle('z'));
  if ((count > 3230 && count < 4370) || digitalRead(startSwitch)) {
    digitalWrite(SOUND_LED, HIGH);
    firstStart = true;
    robotOn = !robotOn;
    resetGyro('z');
  }
  
 // Serial.println(getAngle('z'));
  if(robotOn){
    if (firstStart) {
      FreqCount.end();
      ri = colour_sensor.getColor('r');
    }
	
  	while(!foundFlame && digitalRead(flame_sensor)) {
		naviguessMaze(0.3);
		// Serial.println(getAngle());
       // ultrasonicCheck();
	   ultrasonicArrayCheck();
      // drivetrainCheck();
      
		// hDrive(0.3, 0.0, 0.0);
  	}
	
  	foundFlame = true;
  	// digitalWrite(FLAME_LED, foundFlame);
	digitalWrite(FLAME_LED, true);
  	hDrive(0.0, 0.0, 0.0);

  fan_motor.set(1.0);
  
  	while (true) {
  //		pulseFan();
      if (frontUltrasonic.getDistance() > 3) {
          hDrive(0.3, 0.0, 0.0);
          delay(200);
          hDrive(0.0, 0.0, 0.0);
          delay(400);
      }
  		digitalWrite(FLAME_LED, !digitalRead(flame_sensor));
  	}
  } else{
    digitalWrite(SOUND_LED, LOW);
    firstStart = false;
    hDrive(0.0, 0.0, 0.0);
  }

//  delay(100);
}

void pulseFan() {
	fan_motor.set(1.0);
	delay(3000);
	fan_motor.set(0.0);
  
	delay(500);
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


//  void pidMove(double move, double rotate, double strafe) {
//     if (rotate == 0.0 && move != 0.0) {
//       if (!pidEnabled) {
//         pidEnabled = true;
//         heading = getAngle('z');
//       } else {
//         hDrive(move, getPidOutput(), 0.0);   // Correctional driving
//       }
//     } else {
//       pidEnabled = false;
//       hDrive(move, rotate, strafe);
//     }
//     }
//  }

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

void ultrasonicArrayCheck() {
	Serial.println("====================================");
	Serial.print("LI: ");
	Serial.println(leftArrayInner.getDistance());
	Serial.print("LO: ");
	Serial.println(leftArrayOutter.getDistance());
	Serial.print("RI: ");
	Serial.println(rightArrayInner.getDistance());
	Serial.print("RO: ");
	Serial.println(rightArrayOutter.getDistance());
	delay(300);
}


void driveToWhite() {
  int ri = colour_sensor.getColor('r');
  hDrive(.3,0,0);
  
  while(colour_sensor.getColor('r') > ri - 70) { }
  
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
  return (180 / PI) * (getDistanceLeft() - getDistanceRight()) / 11.5;
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
  // resetGyro('z');
  resetEncoders();
  hDrive(0.0, speed, 0.0);
  
  while (abs(getAngle()) < abs(targetAngle)) {
    Serial.println(getAngle());
  }
  
  hDrive(0.0, 0.0, 0.0);
}

void driveToDistance(double targetDistance, double speed) {
  resetEncoders();
  
  hDrive(speed, 0.0, 0.0);
  
  while (abs(getDistanceLeft()) < abs(targetDistance) && abs(getDistanceRight()) < abs(targetDistance) ) {
    // Serial.println(getDistanceLeft());
  }
  
  hDrive(0.0, 0.0, 0.0);
}

double getPidOutput() {
  return (heading - getAngle('z')) * kP;
}

// void extinguish() {
  // for(int i = 40; i <= 160; i += interval){
    // extingusher_servo.write(i);
    // delay(15*interval);//give time for servo to move

    // if(!digitalRead(flame_sensor))
      // digitalWrite(FLAME_LED, HIGH);
  
    // //turn on fan
    // fan_motor.set(1);
    // while(!digitalRead(flame_sensor)){
      // //while there is a fire
      // delay(1000);   
    // }
    // //turn off fan
    // fan_motor.set(0);
    // digitalWrite(FLAME_LED, LOW);  
  // }
  
  // for(int i = 140; i >=20; i -= interval){
    // extingusher_servo.write(i);
    // delay(15*interval);//give time for servo to move

    // if(!digitalRead(flame_sensor))
      // digitalWrite(FLAME_LED, HIGH);
  
    // //turn on fan
    // fan_motor.set(1);
    // while(!digitalRead(flame_sensor)){
      // //while there is a fire
      // delay(1000);   
    // }
    // //turn off fan
    // fan_motor.set(0);
    // digitalWrite(FLAME_LED, LOW);  
  // }
// }

bool direction = true;
int leftTurns = 0;
int rightTurns = 0;

void naviguessMaze(double swagSpeed) {
	bool frontTriggered = frontUltrasonic.getDistance() < 2;
	bool leftTriggered = leftUltrasonic.getDistance() < 2;
	bool rightTriggered = rightUltrasonic.getDistance() < 2;
	bool backTriggered = backUltrasonic.getDistance() < 2;
  bool rightFavor = rightUltrasonic.getDistance() <7 && rightUltrasonic.getDistance() > 2 && frontUltrasonic.getDistance() < 20;
  bool rightInner = rightArrayInner.getDistance() < 7;
  bool rightOuter = rightArrayOutter.getDistance() < 5;
  bool leftInner = leftArrayInner.getDistance() < 7;
  bool leftOuter = leftArrayOutter.getDistance() < 5;
  int trial = 0; 
  

	
	if (leftTriggered && frontTriggered) {
		hDrive(-swagSpeed, 0.0, 0.0);
		delay(100);
		// hDrive(0.0, swagSpeed , 0.0);
		
		if (rightTurns == 4) {
			turnToAngle(-90, swagSpeed);
			rightTurns = 0;
		} else {
			turnToAngle(90, -swagSpeed);
			rightTurns++;
		}
	} else if (rightTriggered && frontTriggered) {
		hDrive(-swagSpeed, 0.0, 0.0);
		delay(100);
		// hDrive(0.0, -swagSpeed , 0.0);
		
		if (leftTurns == 4) {
			turnToAngle(-90, -swagSpeed);
			leftTurns = 0;
		} else {
			turnToAngle(-90, swagSpeed);
			leftTurns++;
		}
	} else if (leftTriggered && !frontTriggered) {
		// hDrive(swagSpeed - 0.2, -swagSpeed, 0.0);
		turnToAngle(1, -swagSpeed);
	} else if (rightTriggered && !frontTriggered) {
		//hDrive(swagSpeed - 0.2, swagSpeed , 0.0);
		turnToAngle(-1, swagSpeed);
	} else if (frontTriggered && !rightTriggered & !leftTriggered){
		hDrive(-swagSpeed, 0.0, 0.0);
		delay(50);
		if (rightTurns == 4) {
			turnToAngle(-90, swagSpeed);
			rightTurns = 0;
		} else {
			turnToAngle(90, -swagSpeed);
			rightTurns++;
		}
		// turnToAngle(90, -swagSpeed);
		// rightTurns++;
	} else if (backTriggered) {
		hDrive(swagSpeed, swagSpeed, 0.0);
	} else if(rightFavor) {
		hDrive(0.0, 0.0, swagSpeed);
		while (rightUltrasonic.getDistance() > 2.5) { }
		hDrive(0.0, 0.0, 0.0);
   }  if(rightInner  ) {
		 hDrive(-swagSpeed, 0.0, 0.0);
		 delay(1000);
    // turnToAngle(-15, swagSpeed);
  // } else if(leftInner || leftOuter && !frontTriggered) {
		// hDrive(-swagSpeed, 0.0, 0.0);
		// delay(100);
		// turnToAngle(15, -swagSpeed);
  } else {
		hDrive(swagSpeed, 0.0, 0.0);
	}
	
	if (direction) {
		strafeMotor.set(0.3);
	} else {
		strafeMotor.set(-0.3);
	}
	
	direction = !direction;
	
	delay(50);

}

void tankDrive(double leftSpeed, double rightSpeed, double strafeSpeed) {
	leftMotor1.set(constrain(leftSpeed, -1.0, 1.0));
	leftMotor2.set(constrain(leftSpeed, -1.0, 1.0));
	rightMotor1.set(-constrain(rightSpeed, -1.0, 1.0));
	rightMotor2.set(-constrain(rightSpeed, -1.0, 1.0));
	strafeMotor.set(strafeSpeed);
}

void followWall(double targetDistance) {
	if (rightUltrasonic.getDistance() < targetDistance) {
		tankDrive(0.0, 0.3, 0.0);
	} else if (rightUltrasonic.getDistance() > targetDistance) {
		tankDrive(0.3, 0.0, 0.0);
	} else {
		tankDrive(0.3, 0.4, 0.0);
	}
}

double getAngle(char axis) {
  // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
        // other program behavior stuff here
        // .
        // .
        // .
        // if you are really paranoid you can frequently test in between other
        // stuff to see if mpuInterrupt is true, and if so, "break;" from the
        // while() loop to immediately process the MPU data
        // .
        // .
        // .
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        Serial.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;
        
        // display Euler angles in degrees
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetEuler(euler, &q);

        switch (axis) {
          case 'x':
            return (euler[2] * 180/M_PI) - gyroResetValX;
            break;
          case 'y':
            return (euler[1] * 180/M_PI) - gyroResetValY;
            break;
          case 'z':
            return (euler[0] * 180/M_PI) - gyroResetValZ;
            break;
        }
    }
}

void resetGyro(char axis) {
  switch (axis) {
    case 'x':
      gyroResetValX = getAngle('x');
      break;
    case 'y':
      gyroResetValY = getAngle('y');
      break;
    case 'z':
      gyroResetValZ = getAngle('z');
      break;
  }
}
