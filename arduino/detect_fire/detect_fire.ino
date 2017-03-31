#include <Servo.h>
#include "Motor.h"

#define LED_Pin 2
#define flame_sensor 3
#define servo_pin 4
#define interval 10

Motor fan_motor(5,6);


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
  fan_motor.set(1);
  Serial.println("fan is on");
  while(!digitalRead(flame_sensor)){
    //while there is a fire
    delay(1000);   
  }
  Serial.println("Fan is off");
  //turn off fan
  fan_motor.set(0);
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
  
  fan_motor.set(1);
  }
}
  


