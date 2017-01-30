//Brandon Waller
//GW Robotics
//Super Complicated code for IR Fire Sensor

const int SensorPin = 2;
void setup() {
  Serial.begin(9600);
  pinMode(SensorPin, INPUT);
}

void loop() {
  Serial.print(digitalRead(SensorPin));
  Serial.print(" ");
  if(digitalRead(SensorPin) == LOW)
  {
    Serial.println("There is a Fire");
  }
  else
  {
    Serial.println("There is no Fire");
  }
  delay(2000);
}
