#include <EEPROM.h>

char endChar = ',';
unsigned int i = 0;
unsigned int j = 0;
String strings[3];
bool stopRead1 = false; 
bool stopRead2 = false; 
char antwoord = 'o';
char charArray[3][50];

void(* resetFunc) (void) = 0;
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(10);
  //Serial.write(EEPROM.write()) van 1ste tot laatste char voor elke string 
   for(i=0;i<3;i++){
           for(j=0;j<50;j++){
            Serial.print(char(EEPROM.read(j+(i*50))));
           }
           Serial.println();
         }
    Serial.println();
    Serial.println("Geef een Array van 3 Strings in via de terminal gescheiden door een komma en eindigend op een punt.");
    Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()>1&&!stopRead1){      
    for(i=0;i<=2;i++){ 
      endChar = ',';
       if(i>=2){
        endChar = '.';
      }
      else{
        endChar = ',';
      }
           
      strings[i]=Serial.readStringUntil(endChar);
      if(i>=2){
        stopRead1 = true; 
      }
      //Serial.print("i = ");
      //Serial.println(i);
      Serial.println(strings[i]);
     }
     if(stopRead1){
      Serial.println();
      Serial.println("strings opslagen(j/n)");
     }
    }
     while(Serial.available()>1&&!stopRead2){
      //Serial.println("in while lus");
      antwoord = Serial.read();
      //Serial.println(antwoord);
      if(antwoord == 'j'){
        Serial.println("strings opgeslagen");
        Serial.println();Serial.println();
         //zet strings om in array en slaagt ze op in EEPROM (myString.toCharArray(buf, len))
         for(i=0;i<3;i++){
           strings[i].toCharArray(charArray[i],50);
           for(j=0;j<50;j++){
            EEPROM.write(j+(i*50),charArray[i][j]);
           }
         }
         stopRead2 = true;
      }
      else if(antwoord == 'n'){
        Serial.println("niets opgeslagen");
        resetFunc();
         stopRead2 = true;
      }
     }
}
