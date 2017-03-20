/*
 * Brandon Waller
 * GW Robotics HC-SR04 test code
 * print distance from sensor "cm,in"
 */
#define trig 8
#define echo 7

int unit = 0;//0 for cm, 1 for inches
long duration, distance;

void setUnit(int u)
{
  unit = (u == 1 ? 1 : u == 0 ? 0 : unit = unit);
    
}
void setup()  
{
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  Serial.begin(9600);
}
void loop()
{
  digitalWrite(trig, LOW);
  delayMicroseconds(2);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);

  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);

  
  Serial.println(unit == 1 ? distIn() : distCM());

  delay(2000);
}

float distCM()
{
  return duration / 2. * .034;
  //ping time divided by 2 (account for both directions) * speed of sound cm/us
}
float distIn()
{
  return duration / 2. * .013504;
}

