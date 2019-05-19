const int digits[] = {13,A0,4,5};
const int anodes[] = {6,7,8,9,10,11,12}; // {A,B,C,D,E,F,G}
const int inA = 2;
const int inB = 3;
const int num_array[10][7] = {  { 1,1,1,1,1,1,0 },    // 0
                          { 0,1,1,0,0,0,0 },    // 1
                          { 1,1,0,1,1,0,1 },    // 2
                          { 1,1,1,1,0,0,1 },    // 3
                          { 0,1,1,0,0,1,1 },    // 4
                          { 1,0,1,1,0,1,1 },    // 5
                          { 1,0,1,1,1,1,1 },    // 6
                          { 1,1,1,0,0,0,0 },    // 7
                          { 1,1,1,1,1,1,1 },    // 8
                          { 1,1,1,0,0,1,1 }};   // 9
int curr = HIGH;
int prev = HIGH;
int i = 0;
int j = 0;
int k = 0;
bool printI = false;

void setup() {
  // put your setup code here, to run once:
  pinMode (inA,INPUT);
  pinMode (inB,INPUT);
  digitalWrite(inA,HIGH);
  digitalWrite(inB,HIGH);
  for(k=0; k<4; k++){
    pinMode(digits[k], OUTPUT);
     attachInterrupt(digitalPinToInterrupt(inA),aHIGH,RISING);
     Serial.begin(9600);
  }
  
  for(k=0; k<7; k++){
    pinMode(anodes[k], OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if(printI){
        Serial.println(i);
        printI = false;
      }
   for(k=0;k<7;k++){
      digitalWrite(anodes[k],num_array[i][k]);
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
