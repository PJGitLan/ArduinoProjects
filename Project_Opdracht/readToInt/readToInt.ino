String serialAns ="Nan";
int number;
int i = 0;
int counter = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(100);
}

void loop() {
  // put your main code here, to run repeatedly:
     if(Serial.available() > 0){
      serialAns = Serial.readString();
      
      counter = 0;
      for(i=0; i<serialAns.length(); i++){
        if(isDigit(serialAns[i])){
          counter++;
        }
      }

      if(serialAns.length()-1 == counter){
        number = serialAns.toInt();
      }
      else{
        number = -99;
      }
      
      if(number>=2&&number<=400){
        Serial.print("number = ");
        Serial.println(number);
      }
    //Serial.flush();
  }
}
