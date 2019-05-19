const int buttonPin1 = 2;
const int buttonPin2 = 3;
const int led = 4;

volatile bool ledState = false;

void setup() {
  // put your setup code here, to run once:
  attachInterrupt(digitalPinToInterrupt(buttonPin1),ledAan,RISING);
  attachInterrupt(digitalPinToInterrupt(buttonPin2),ledUit,RISING);
  
  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly: 
  digitalWrite(led, ledState);
}
  
void ledAan(){
   ledState = true;
}

void ledUit(){
    ledState = false;
}
