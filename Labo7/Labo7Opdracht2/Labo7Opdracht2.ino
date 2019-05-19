const int button = 2;
char charVar = 'p';
char *c = 0x0100;
bool pushed = false;
bool einde = false;
int i = 0;
int j =0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(button), memDump, RISING);
  
  
    
}

void loop() {
  // put your main code here, to run repeatedly:
  if(pushed){
    if(c >= 0x08FF){
      Serial.println("----------- einde ------------");
      einde = true;
      pushed = false;
    }
    else{
          for(j=0;j<20;j++){
            Serial.print(int(c),HEX);
          Serial.print("  ");
          //c++;
          for(i=0;i<16;i++){
            if(*c != NULL){
              Serial.print(*c);
            }
            else{
              Serial.print('X');
            }
            c++;
              Serial.print("  ");
          }
          Serial.println();
          pushed = false;
        }
      }
   }
    
}

void memDump(){
  if(!einde){
    if(!pushed){
     pushed = true;
    }
  }
}
