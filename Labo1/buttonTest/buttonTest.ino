const int button = 2;
const int led = 4;

bool buttonState = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);
  pinMode(button, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonState = digitalRead(button);
  if (buttonState == HIGH){
    digitalWrite(led, HIGH);
  }
  else{
    digitalWrite(led, LOW); 
  }
}
