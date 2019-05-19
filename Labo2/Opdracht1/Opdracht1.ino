const int pin1 = 2;
const int led = 13;

void setup() {
  // put your setup code here, to run once:
  pinMode(pin1, INPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool inputState = digitalRead(pin1);
  
  if(inputState){
      digitalWrite(led, HIGH);
    }
  else{
      digitalWrite(led, LOW);
    }
}
