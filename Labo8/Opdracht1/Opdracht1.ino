const int knop1 = 2;
const int knop2 = 3;

bool einde = true;
bool stuur1 = false;
bool stuur2 = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(knop1), onKnop1, RISING);
  attachInterrupt(digitalPinToInterrupt(knop2), onKnop2, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(stuur1){
    //Serial.flush();
    Serial.write('1');
    delay(50);
    stuur1 = false;
    einde = true;
  }
  if(stuur2){
    //Serial.flush();
    Serial.write('2');
    delay(100);
    stuur2 = false;
    einde = true;
  }
  
}

void onKnop1(){
  if(einde){
    einde = false;
    stuur1 = true;
  }
}

void onKnop2(){
  if(einde){
    einde = false;
    stuur2 = true;
  }
}
