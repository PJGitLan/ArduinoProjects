const int ledGroen = 2;
const int ledGeel = 3;
const int ledRood = 13;

char charIN = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledGroen, OUTPUT);
  pinMode(ledGeel, OUTPUT);
  pinMode(ledRood, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    charIN = Serial.read();
    switch (charIN) {
      case '0':
        digitalWrite(ledRood, LOW);
        break;
      case '1':
        digitalWrite(ledRood, HIGH);
        break;
      case '2':
        digitalWrite(ledGroen, LOW);
        break;
       case '3':
        digitalWrite(ledGroen, HIGH);
        break;
       case '4':
        digitalWrite(ledGeel, LOW);
        break;
       case '5':
       digitalWrite(ledGeel, HIGH);
       break;
    }
  }
}
