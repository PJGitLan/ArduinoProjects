const int pin1 = 2; //button
const int led = 13;

bool prev = true;

void setup() {
  // put your setup code here, to run once:
  pinMode(pin1, INPUT);
  pinMode(led, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool inputState = digitalRead(pin1);
  
  if(inputState){
    if(!prev){
      prev = HIGH;
      digitalWrite(led, HIGH);
    }
    else{
      prev = LOW;
      digitalWrite(led, LOW);
    }
  

    inputState = digitalRead(pin1);
    while(inputState){
      delayMicroseconds(10);//software-oplossingen voor dender
      inputState = digitalRead(pin1);
    }
  }

}
