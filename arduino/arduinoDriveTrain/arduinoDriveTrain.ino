
  // 3 controllers
  //listed from front
  //controller1
  int controller1A1 = 13;
  int controller1A2 = 12;
  
  int controller1B1 = 11;
  int controller1B2 = 10;
  
  //controller2
  int controller2A1 = 9;
  int controller2A2 = 8;
  
  int controller2B1 = 7;
  int controller2B2 = 6;
  
  //controller3
  int controller3A1 = 5;
  int controller3A2 = 4;
  
  int controller3B1 = 3;
  int controller3B2 = 2;
  
  void setup(){
  // set all the motor control pins to outputs
  pinMode(controller1A1, OUTPUT);
  pinMode(controller1A2, OUTPUT);
  pinMode(controller1B1, OUTPUT);
  pinMode(controller1B2, OUTPUT);
  
  pinMode(controller2A1, OUTPUT);
  pinMode(controller2A2, OUTPUT);
  pinMode(controller2B1, OUTPUT);
  pinMode(controller2B2, OUTPUT);

  pinMode(controller3A1, OUTPUT);
  pinMode(controller3A2, OUTPUT);
  pinMode(controller3B1, OUTPUT);
  pinMode(controller3B2, OUTPUT);

  //NEED TO ASSIGN VALUES 
  int frontRightMotor = 0;
  int frontLeftMotor = 0;
  int backRightMotor = 0;
  int backLeftMotor = 0;
  int middleMotor = 0;
  int pump = 0;
  
}
//
//dir is direction, vel is velocity, duration
//velocity is from 0 to 255
//direciton is 1 or -1
  void motorSpeed(int controller, int dir, int vel){
 
    if( dir = 1){
        analogWrite(controller, vel);
        analogWrite(controller, 0);
    }
    
    else{
        analogWrite(controller, 0);
        analogWrite(controller, vel);
    }
  
  }
  
void loop()
{
  //example code to make controller1A1 go forward at 200
  motorSpeed(controller1A1, 1, 200);
 
}


