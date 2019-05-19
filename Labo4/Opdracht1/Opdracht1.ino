const int inA = 2;
const int inB = 3;

int curr = HIGH;
int prev = HIGH;
volatile int i = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode (inA,INPUT);
  pinMode (inB,INPUT);
  
  digitalWrite(inA,HIGH);
  digitalWrite(inB,HIGH);

  Serial.begin (9600); 
}

void loop() {
  curr = digitalRead(inA);
  if (curr != prev){
    if(digitalRead(inB) == prev){
      i++;
      }

     else {
      i--;
      }
  
  Serial.println(i);
    
  }
  prev = curr;
}

  
