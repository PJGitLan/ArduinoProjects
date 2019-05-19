const int grLed = 4;
const int rdLed = 5;
const int trimmer = A0;

int number = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(grLed, OUTPUT);
  pinMode(rdLed, OUTPUT);
  pinMode(trimmer, INPUT);

  Serial.begin(9600);
  Serial.setTimeout(10);

  Serial.println("Geef een getal van 0 tot 1023: ");
}

void loop() {
  // put your main code here, to run repeatedly:
    while(Serial.available() > 1){
    number = Serial.parseInt();
    
//    Serial.print("number =");
//    Serial.println(number);
//    Serial.println(analogRead(trimmer));
    if(number>=0&&number<=1023){
     
      if(number<analogRead(trimmer)){
        digitalWrite(grLed,HIGH);
        digitalWrite(rdLed,LOW);
      }
      else if(number>analogRead(trimmer)){
        digitalWrite(grLed,LOW);
        digitalWrite(rdLed,HIGH);
      }
    }
    else{
       Serial.println("Getal ligt niet tussen 0 en 1023");
    }
  }
}
