#include <Servo.h>
#include <NewPing.h>

Servo servo;

const int trig = 3;
const int echo = 4;

int distance = 0;

int angle = 0;
int i = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo.attach(2);
  
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  NewPing sonar(trig, echo, 400);
  Serial.print("Distance: ");
  distance = sonar.ping_cm();
  Serial.print(distance);
  Serial.println(" cm");
  angle = 0;
  if(distance<20){
    for(i=distance;i<20;i++){
      angle++;
    }
   angle = map(angle, 0, 19, 91 , 180);
   //angle = 135;
  }

  else if(distance>20){
    for(i=distance;i>20;i--){
      angle++;
    }
   angle = map(angle, 400, 0, 0 , 89);
   //angle = 45;
  }

  else if(distance==20){
    angle = 90;
  } 

  servo.write(angle);
  delay(15);
  Serial.print("Angle: ");
  Serial.println(angle);
}
