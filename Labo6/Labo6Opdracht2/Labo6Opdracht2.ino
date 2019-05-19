//#include <Servo.h>
#include <NewPing.h>

const int trig = 2;
const int echo = 3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  NewPing sonar(trig, echo, 400);
  Serial.print("Distance: ");
  Serial.print(sonar.ping_cm());
  Serial.println(" cm");
}
