String serialAns ="Nan";
int number;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
     while(Serial.available() > 1){
      number = Serial.parseInt();
      if(number>=2&&number<=400){
        Serial.print("number =");
        Serial.println(number);
      }
    Serial.flush();
  }
}
