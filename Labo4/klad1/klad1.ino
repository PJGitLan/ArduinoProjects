const int inA = 2;
const int inB = 3;
  
int i = 0;
char prev = '0'; 

void setup() {
  // put your setup code here, to run once:
  pinMode(inA, INPUT);
  pinMode(inB, INPUT);
    
  inA = LOW;
  inB = LOW;

  Serial.begin ( 9600 );
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(inA)&& prev == 'A'){
    prev = 'A';
  }

  if(digitalRead(inB)&& prev == 'B'){
    prev = 'B';
  }
    
  if(digitalRead(inA)&& prev == 'B'){
    prev = 'A';
    i++;
  }

  if(digitalRead(inB)&&prev == 'A'){
    prev = 'B';
    i--;
  }
  if(digitalRead(inA)&&prev == '0'){
    prev = 'A';
  }

  if(digitalRead(inB)&&prev == '0'){
    prev = 'B';
  }

  Serial.println(i);
}
