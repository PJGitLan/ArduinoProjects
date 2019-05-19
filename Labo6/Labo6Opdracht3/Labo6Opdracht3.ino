#include <Servo.h>

Servo servo;
const int pot = A0;
int angle = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servo.attach(2);
}

void loop() {
  // put your main code here, to run repeatedly:
  angle = map(analogRead(pot), 0, 1023, 0, 180);
  servo.write(angle);
  delay(15);
  Serial.println(analogRead(pot));
}
