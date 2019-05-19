const int leds[] = {2,3,4,5,6};
const int V_in = A3;

int voltage = 0;
int i = 0;

void setup() {
  // put your setup code here, to run once:
  int i;
  for(i = 0; i < 5; i++){
    pinMode(leds[i], OUTPUT);
  }
  pinMode(V_in, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  voltage = analogRead(V_in)*5/1024;

  for(i=0; i<5; i++){
    digitalWrite(leds[i], LOW);
  }
  
  for(i=0; i<voltage; i++){
    digitalWrite(leds[i], HIGH);
  }
  
}
