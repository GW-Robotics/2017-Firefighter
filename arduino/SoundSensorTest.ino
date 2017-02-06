#define sensorPin A0

void setup() {
  //pinMode(sensorPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  
  Serial.println(analogRead(sensorPin));
  delay(20);
}
