const int arrayLength = 5;
const int leds[arrayLength] = {2,3,4,5,6};
const int V_in = A3;

int speedSet = 0;
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
  speedSet = analogRead(V_in);
  
  for(i=0; i<arrayLength; i++){
    digitalWrite(leds[i-1], LOW);
    digitalWrite(leds[i], HIGH);
    if(i != arrayLength-1){
      delay(speedSet);
    }
  }

   for(i=1; i<arrayLength; i++){
    digitalWrite(leds[arrayLength-i+1], LOW);
    digitalWrite(leds[arrayLength-i], HIGH); 
    delay(speedSet);   
  }
  digitalWrite(leds[1], LOW);
  digitalWrite(leds[0], HIGH);
}
