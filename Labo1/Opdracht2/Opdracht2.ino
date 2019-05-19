const int buttonPin1 = 2;
const int buttonPin2 = 3;
const int led = 4;

bool buttonState1 = 0;
bool buttonState2 = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool buttonState1 = digitalRead(buttonPin1);
  bool buttonState2 = digitalRead(buttonPin2);
 
  
  if(buttonState1){
    digitalWrite(led, HIGH);
  }

  else if(buttonState2){
    digitalWrite(led, LOW);
  }
}
