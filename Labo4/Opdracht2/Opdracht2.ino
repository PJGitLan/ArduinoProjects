const int inA = 2;
const int inB = 3;
const int leds[] = {4,5,6,7,8,9,10,11,12,13};

int curr = HIGH;
int prev = HIGH;
int i = 0;
int j = 0;

bool printI = false;

void setup() {
  // put your setup code here, to run once:
  pinMode (inA,INPUT);
  pinMode (inB,INPUT);

  for(j=0;j<10;j++){
      pinMode(leds[j], OUTPUT);
    }
    j = 0;

  digitalWrite(inA,HIGH);
  digitalWrite(inB,HIGH);

  attachInterrupt(digitalPinToInterrupt(inA),aHIGH,RISING);
  //attachInterrupt(digitalPinToInterrupt(inA),aLOW,FALLING);
  //attachInterrupt(digitalPinToInterrupt(inA),bHIGH,RISING);
  //attachInterrupt(digitalPinToInterrupt(inA),bLOW,FALLING);

  Serial.begin (9600);
}

void loop() {
  // put your main code here, to run repeatedly:
      if(printI){
        Serial.println(i);
        printI = false;
      }
      for(j=0;j<i;j++){
             digitalWrite(leds[j], HIGH);
           }
}

void aHIGH()
 {
  curr = digitalRead(inA); 
 
   if (curr != prev){     
     
     if (digitalRead(inB) != curr) { 
       if(i<8){
       i++;
      }
       printI = true;
     } else {
      if(i>0){
       i--;
      }
       printI = true;
     }
     
   } 
   prev = curr;
   
 } 
