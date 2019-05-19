
#include <Servo.h>

const int trig = 2;
const int echo = 3;

long duration;
int distance;
int prev;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  //digitalWrite(trig, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(trig, HIGH);
  delayMicroseconds(2);
  digitalWrite(trig, LOW);

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  duration = pulseIn(echo, HIGH);
  distance = duration*0.034/2;
  
  if(prev == distance){
    if(distance > 400 || distance < 2){
      Serial.println("Error");
    }

    else{
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");
    }
  }
  
  prev = distance;
}
