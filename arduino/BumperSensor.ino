/*
 * Brandon Waller
 * GW Robotics
 * Super difficult arduino code for the bumper sensor
 * 
 * Connect pin 3 to 5v
 * Connect pin to `1a 47k resistor in series with ground in parrallel with a digital pin
 */

const int snesorPin = 4;
int presses = 1;
void setup() {
  // put your setup code here, to run once:
  pinMode(snesorPin, INPUT);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  if(digitalRead(snesorPin) == HIGH)
  {
    Serial.println(presses);
    presses++;
    delay(1000);
  }
}
