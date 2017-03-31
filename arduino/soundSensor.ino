
int sensorPin = A0;    
int ledPin = 13;      
int sensorValue = 0;
bool robotOn = false;

void setup() {
  // declare the ledPin as an OUTPUT:
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);
  Serial.begin(112500);
}

void soundStart(){
  
  sensorValue = analogRead(sensorPin);
  Serial.println("Sensor Value: " + (String)sensorValue);
  Serial.print( "  RobotOn: " +  (String)robotOn) ;
  delay(100);
  
    if(sensorValue > 500) {
      robotOn = true;
    }
    while(robotOn){
      for(int i = 0; i <100; i++){
       digitalWrite(ledPin, HIGH);
       Serial.println( "RobotOn: TRUE") ;
      }
    }    
}
void loop() {
 soundStart();
}
