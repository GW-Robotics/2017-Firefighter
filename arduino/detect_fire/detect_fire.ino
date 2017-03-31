#include <Servo.h>

#define LED_Pin 2
#define flame_sensor 3
#define servo_pin 4
#define interval 10
Servo extingusher_servo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_Pin, OUTPUT);
  pinMode(flame_sensor, INPUT);

  extingusher_servo.attach(servo_pin);
  extingusher_servo.write(0);
}
void put_out_fire(){
  Serial.println("There is a fire");
  digitalWrite(LED_Pin, HIGH);
  //turn on fan
  while(!digitalRead(flame_sensor)){
    //while there is a fire
    delay(1000);   
  }
  //turn off fan
  digitalWrite(LED_Pin, LOW);
}
void loop() {
  // put your main code here, to run repeatedly:
  int i = 0;
  for(i; i <= 180; i += interval){
    extingusher_servo.write(i);
    delay(15*interval);//give time for servo to move

    if(!digitalRead(flame_sensor))
      put_out_fire();    
  }
  for(i; i >=0; i -= interval){
    extingusher_servo.write(i);
    delay(15*interval);//give time for servo to move

    if(!digitalRead(flame_sensor))
      put_out_fire();  
  }
  
}

