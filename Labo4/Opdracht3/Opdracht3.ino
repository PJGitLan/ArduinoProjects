const int digits[] = {2,3,4,5};
const int anodes[] = {6,7,8,9,10,11,12}; // {A,B,C,D,E,F,G}

int i = 0;
int j = 0;

void setup() {
  // put your setup code here, to run once:
  for(i=0; i<4; i++){
    pinMode(digits[i], OUTPUT);
  }
  
  for(i=0; i<7; i++){
    pinMode(anodes[i], OUTPUT);
  }
}

void loop() {
 // put your main code here, to run repeatedly:
 for(j=0;j<4;j++){
   digitalWrite(digits[j],HIGH);
 }
 
 for(j=0;j<4;j++){
   digitalWrite(digits[j],LOW);
   for(i=0;i<7;i++){
      digitalWrite(anodes[i],HIGH);
      delay(1000);
      digitalWrite(anodes[i],LOW);
    }
    digitalWrite(digits[j],HIGH);
  }
}
