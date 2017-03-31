//3 controllers
//listed from front
//controller1 strafe
int controller1B1 = 12;
int controller1B2 = 11;

//controller2 right back then front
int controller2A1 = 10;
int controller2A2 = 9;  

int controller2B1 = 8;
int controller2B2 = 7;

//controller3 left back then front 
int controller3A1 = 6;
int controller3A2 = 5;

int controller3B1 = 4;
int controller3B2 = 3;

//ultrasonic 
const int trigPinF = 29;
const int echoPinF = 28;
long durationF;
int distanceF;
const int trigPinL = 27;
const int echoPinL = 26;
long durationL;
int distanceL;
const int trigPinR = 25;
const int echoPinR = 24;
long durationR;
int distanceR;

//Boolean Values for Wall Detection
boolean Fhit = false;
boolean Rhit = false;
boolean Lhit = false;
boolean Sweet = false;

//boolean Values for Flame
boolean Flame = false;

//IR Values for Flame
const int SensorPin = 2;

void setup() 
{
  Serial.begin(9600);
  // set all the motor control pins to outputs
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

  //ultrasonic setup
  pinMode(trigPinF, OUTPUT); 
  pinMode(echoPinF, INPUT);

  pinMode(trigPinL, OUTPUT); 
  pinMode(echoPinL, INPUT);
  
  pinMode(trigPinR, OUTPUT); 
  pinMode(echoPinR, INPUT);    

  //flame setup
  pinMode(SensorPin, INPUT);
}

void loop() 
{
    read_sensors();
    analyze();
    //run_motor();
    //time_function();
}

void read_sensors()
{
  //if(digitalRead(SensorPin == LOW)) Flame = true;
  //else
  //{
  //  Flame = false;
  //}
  
  // Clears the trigPin
  digitalWrite(trigPinL, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinL, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinL, LOW);
  durationL = pulseIn(echoPinL, HIGH);
      
  digitalWrite(trigPinR, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinR, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinR, LOW);
  durationR = pulseIn(echoPinR, HIGH);
      
  digitalWrite(trigPinF, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinF, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinF, LOW);
  durationF = pulseIn(echoPinF, HIGH);

  // Calculating the distance in Inches
  distanceF= durationF*0.0135/2;
  distanceL= durationL*0.0135/2;
  distanceR= durationR*0.0135/2;
  Serial.println(distanceL);
}

void analyze()
{
  if(distanceF < 10) Fhit = true;
  else if((distanceL > 4)&&(distanceR > 4)) Sweet = true;
  else if(distanceL < 4)
  {
    Lhit = true;
    Fhit = distanceF < 10;
  }
  else if(distanceR < 4) 
  {
    Rhit = true;
    Fhit = distanceF < 10;
  }
  else
  {
    Fhit = false;
    Lhit = false;
    Rhit = false;
    Sweet = false;
  }

  distanceF= 0;
  distanceL= 0;
  distanceR= 0;
}

void run_motor()
{
  if (Flame) hard_stop();
  else if(Sweet) forward();
  else if(Lhit && Fhit) hardright();
  else if(Rhit && Fhit) hardleft();
  else if(Lhit) right(); 
  else if(Rhit) left();
  else if(Fhit) hardright();
  else arc();

  Flame = false;
  Sweet = false;
  Lhit = false;
  Fhit = false;
  Rhit = false;
}

void flame()
{
  //code for extinguisher
}

//void time_function()
//{
//    delayMicroseconds(25);
//}

void forward()
{
    analogWrite(controller2B2, 100);
    analogWrite(controller2B1, 0);
    analogWrite(controller3B2, 100);
    analogWrite(controller3B1, 0);    
    analogWrite(controller2A2, 100);
    analogWrite(controller2A1, 0);
    analogWrite(controller3A2, 100);
    analogWrite(controller3A1, 0);    
}

void right()
{
    analogWrite(controller2B2, 0);
    analogWrite(controller2B1, 0);
    analogWrite(controller3B2, 100);
    analogWrite(controller3B1, 0);    
    analogWrite(controller2A2, 0);
    analogWrite(controller2A1, 0);
    analogWrite(controller3A2, 100);
    analogWrite(controller3A1, 0);
}

void left()
{
    analogWrite(controller2B2, 100);
    analogWrite(controller2B1, 0);
    analogWrite(controller3B2, 0);
    analogWrite(controller3B1, 0);    
    analogWrite(controller2A2, 100);
    analogWrite(controller2A1, 0);
    analogWrite(controller3A2, 0);
    analogWrite(controller3A1, 0); 
}

void hard_stop()
{
    analogWrite(controller2B2, 0);
    analogWrite(controller2B1, 0);
    analogWrite(controller3B2, 0);
    analogWrite(controller3B1, 0);    
    analogWrite(controller2A2, 0);
    analogWrite(controller2A1, 0);
    analogWrite(controller3A2, 0);
    analogWrite(controller3A1, 0);
    flame();
}

void hardleft()
{
    analogWrite(controller2B2, 100);
    analogWrite(controller2B1, 0);
    analogWrite(controller3B2, 0);
    analogWrite(controller3B1, 100);    
    analogWrite(controller2A2, 100);
    analogWrite(controller2A1, 0);
    analogWrite(controller3A2, 0);
    analogWrite(controller3A1, 100);  
}

void hardright()
{
    analogWrite(controller2B2, 0);
    analogWrite(controller2B1, 100);
    analogWrite(controller3B2, 100);
    analogWrite(controller3B1, 0);    
    analogWrite(controller2A2, 0);
    analogWrite(controller2A1, 100);
    analogWrite(controller3A2, 100);
    analogWrite(controller3A1, 0);  
}

void arc()
{
    analogWrite(controller2B2, 25);
    analogWrite(controller2B1, 0);
    analogWrite(controller3B2, 150);
    analogWrite(controller3B1, 0);    
    analogWrite(controller2A2, 150);
    analogWrite(controller2A1, 0);
    analogWrite(controller3A2, 25);
    analogWrite(controller3A1, 0); 
}

