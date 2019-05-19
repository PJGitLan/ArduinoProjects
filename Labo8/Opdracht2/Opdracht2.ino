const int rood = 13;
const int groen = 2;
const int blauw = 3;
char aBuffer[4] = {};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(rood, OUTPUT);
  pinMode(groen, OUTPUT);
  pinMode(blauw, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.readBytesUntil('\n',aBuffer,4);
  if(aBuffer[0]== 'r' && aBuffer[1]=='='){
    int state = aBuffer[2]-'0';
    digitalWrite(rood,state);
  }

  else if(aBuffer[0]== 'g' && aBuffer[1]=='='){
    int state = aBuffer[2]-'0';
    digitalWrite(groen,state);
  }

  else if(aBuffer[0]== 'b' && aBuffer[1]=='='){
    int state = aBuffer[2]-'0';
    digitalWrite(blauw,state);
  }

}
