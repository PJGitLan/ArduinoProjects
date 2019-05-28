//Pieter-Jan Lanneer
#include <LiquidCrystal.h> //Lcd library
#include <NewPing.h> //Ultrasone sensor library
#include <Servo.h> //Servo library
#include <EEPROM.h> //EEPROM library

//EEPROM locations
int EminLength = 0;
int EmaxLength = 50;
int Ehoek = 100;

//Lcd pins & setup
const int rs = 12, en = 11, d4 = 7, d5 = 8, d6 = 9, d7 = 10;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//LiquidCrystal lcd(rs, en, d7, d6, d5, d4 );

//Rotary pins
const int pinA = 2;
const int pinB = 4;

//button pin
const int buttonPin = 13;

//Led pins
const int leds[] = {A0, A1, A2, A3, A4};
const int ledMode = A5; //extra led to indicate measuring mode

//Sonar pins &setup
const int echoPin = 3;
const int trigPin = 5;
int maxDistance = 400; //Shorter maxDistance makes quicker pings/measurements but less range
NewPing sonar(trigPin, echoPin, maxDistance);

//Servo setup
Servo servo;
const int servoPin = 6;

//Variable that's updated by the ISR "updatePosition()"
volatile int rotaryPosition, prevPosition = 0;

//flags
bool optionSelected = false;

//variables
int i = 0;
int optionsSize = 0;
int hoek = EEPROM.read(Ehoek);

unsigned int previousDistance = 0;

float scale = 0;
float minimumLength = EEPROM.read(EminLength);
float maximumLength = EEPROM.read(EmaxLength);

//^^^ Global vars etc ^^^

void setup() {
  Serial.begin(9600);
  //Serial.setTimeout(100);
  lcd.begin(16, 2); //Initiates 16x2 lcd screen
  servo.attach(servoPin); //Attaches servo to right pin

  //define pins as in- or outputs
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(buttonPin, INPUT);

  for (i = 0; i < 5; i++) {
    pinMode(leds[i], OUTPUT);
  }
  pinMode(ledMode, OUTPUT);

  //attaching ISR "updatePostion" to pinA
  attachInterrupt(digitalPinToInterrupt(pinA), updatePosition, LOW);//FALLING gives errors

  calculatingScale();
}

void loop() {
  // put your main code here, to run repeatedly:
  startMenu();
}

void clearLeds() {
  for (i = 0; i < 5; i++) { //Puts every led from the array Leds on LOW
    digitalWrite(leds[i], LOW);
  }
  digitalWrite(ledMode, LOW);
}

void printMenu(String options[], int optionsSize) {
  Serial.println(options[rotaryPosition]);
  if (rotaryPosition < 0) { //scrolling down on first option selects last option
    rotaryPosition = optionsSize - 1;
  }
  else {
    rotaryPosition = rotaryPosition % optionsSize; //Makes sure rotaryPostion doesn't becomes greater then optionsSize
  }

  //Prints options to the lcd screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">");
  lcd.setCursor(1, 0);
  lcd.print(options[rotaryPosition]);
  lcd.setCursor(1, 1);
  if (rotaryPosition + 1 >= optionsSize) { //Latest option wil be followed up by first option
    lcd.print(options[0]);
  }
  else {
    lcd.print(options[rotaryPosition + 1]); //shows next available option
  }
}

void startMenu() {
  optionsSize = 2;
  String options[optionsSize] = {"afstand meten", "hoeken meten"};
  printMenu(options, optionsSize);

  while (!optionSelected) { //Loop that checks for button presses and rotary changes. Updates screen if needed
    checkButton();
    if (rotaryTurned()) {
      printMenu(options, optionsSize);
    }
  }

  if (optionSelected) { //Select next menu
    optionSelected = false;
    int temp = rotaryPosition;
    rotaryPosition = 0;
    switch (temp) {
      case 0:
        afstandMeten();
        break;
      case 1:
        hoekenMeten();
        break;
    }
  }
}

void afstandMeten() {
  digitalWrite(ledMode, HIGH);
  optionsSize = 5;
  String options[optionsSize] = {"minimum meetwaarde instellen  ", "maximum meetwaarde instellen  ", "bekijken verdeling over de led's  ", "meten activeren", "terug naar het hoofdmenu  "};
  printMenu(options, optionsSize);

  while (!optionSelected) {
    checkButton();
    scrollMenu(options[rotaryPosition],1);
    if (rotaryTurned()) {
      printMenu(options, optionsSize);
    }
  }

  if (optionSelected) {
    optionSelected = false;
    int temp = rotaryPosition;
    rotaryPosition = 0;
    switch (temp) {
      case 0:
        minimumMeetwaarde();
        break;
      case 1:
        maximumMeetwaarde();
        break;
      case 2:
        verdelingLeds();
        break;
      case 3:
        metenActiveren();
        break;
      case 4:
        clearLeds();
        startMenu();
        break;
    }
  }
}

void hoekenMeten() {
  optionsSize = 3;
  String options[optionsSize] = {"instellen van de hoek  ", "hoek activeren", "terug naar het hoofdmenu  "};
  printMenu(options, optionsSize);

  while (!optionSelected) {
    checkButton();
    scrollMenu(options[rotaryPosition],1);
    if (rotaryTurned()) {
      printMenu(options, optionsSize);
    }
  }

  if (optionSelected) {
    optionSelected = false;
    int temp = rotaryPosition;
    rotaryPosition = 0;

    switch (temp) {
      case 0:
        instellenHoek();
        break;
      case 1:
        hoekActiveren();
        break;
      case 2:
        startMenu();
        break;
    }
  }
}

void minimumMeetwaarde() {

  digitalWrite(ledMode, HIGH);
  /*max()checks if position is greater then 2. If not it returns 2.
    min()checks if position is smaller then 400. If not it returns 400.*/
  rotaryPosition = min(400, max(2, rotaryPosition));
  //Prints initial screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Minimum meetwaarde:");
  lcd.setCursor(6, 1);
  lcd.print(rotaryPosition);

  while (!optionSelected) {
    checkButton();
    scrollMenu("Minimum meetwaarde: ",0);
    if (rotaryTurned()) {
      /*max()checks if position is greater then 0. If not it returns 0.
        min()checks if position is smaller then 100. If not it returns 100.*/
      rotaryPosition = min(400, max(2, rotaryPosition));
      lcd.setCursor(6, 1);
      lcd.print("   ");
      lcd.setCursor(6, 1);
      lcd.print(rotaryPosition);
    }
  }

  if (optionSelected) {
    optionSelected = false;
    if (rotaryPosition < maximumLength) {
      minimumLength = rotaryPosition;
      EEPROM.update(EminLength, minimumLength); //Changes EEPROM data if it's different
      calculatingScale();
      rotaryPosition = 0;
      afstandMeten();
    }
    else { //Writes error message
      digitalWrite(ledMode, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error:");
      lcd.setCursor(0, 1);
      lcd.print("minimum>maximum");
      delay(3000);
      minimumMeetwaarde();
    }
  }
}

void maximumMeetwaarde() {

  digitalWrite(ledMode, HIGH);
  /*max()checks if position is greater then 2. If not it returns 2.
    min()checks if position is smaller then 400. If not it returns 400.*/
  rotaryPosition = min(400, max(2, rotaryPosition));
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Maximum meetwaarde:");
  lcd.setCursor(6, 1);
  lcd.print(rotaryPosition);

  while (!optionSelected) {
    checkButton();
    scrollMenu("Maximum meetwaarde: ",0);
    if (rotaryTurned()) {
      /*max()checks if position is greater then 2. If not it returns .
        min()checks if position is smaller then 400. If not it returns 400.*/
      rotaryPosition = min(400, max(2, rotaryPosition));
      lcd.setCursor(6, 1);
      lcd.print("   ");
      lcd.setCursor(6, 1);
      lcd.print(rotaryPosition);
    }
  }

  if (optionSelected) {
    optionSelected = false;
    if (rotaryPosition > minimumLength) {
      maximumLength = rotaryPosition;
      EEPROM.update(EmaxLength, maximumLength);
      calculatingScale();
      rotaryPosition = 0;
      afstandMeten();
    }
    else {
      digitalWrite(ledMode, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error:");
      lcd.setCursor(0, 1);
      lcd.print("maximum<minimum");
      delay(3000);
      maximumMeetwaarde();
    }
  }
}

void calculatingScale() {
  scale = (maximumLength - minimumLength) / 4;
}

void verdelingLeds() {
  String firstLine = "";
  String secondLine = "";

  for (int i = 0; i < 2; i++) {
    firstLine = firstLine + "L" + (i + 1) + ":" + (minimumLength + scale * i) + " ";
  }

  for (int i = 2; i < 5; i++) {
    secondLine = secondLine + "L" + (i + 1) + ":" + (minimumLength + scale * i) + " ";
  }
  
  digitalWrite(ledMode, HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(firstLine);
  lcd.setCursor(0, 1);
  lcd.print(secondLine);
  
  while (!optionSelected) {
    checkButton();
    scrollScreen(firstLine, secondLine);
  }
  
  if (optionSelected) {
  optionSelected = false;
  rotaryPosition = 0;
  afstandMeten();
}
}

void metenActiveren() {
  while (!optionSelected) {
    checkButton();
    //delay(35);
    unsigned int echoTime = sonar.ping_median();
    unsigned int distanceCm = sonar.convert_cm(echoTime);

    digitalWrite(ledMode, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Afstand in cm");
      lcd.setCursor(0, 1);
      lcd.print(distanceCm);

    if (distanceCm != previousDistance) {
      previousDistance =  distanceCm;

      for (int i = 0; i < 5; i++) {
        if (minimumLength + scale * i <= distanceCm) {
          clearLeds();
          digitalWrite(leds[i], HIGH);
        }
        if (minimumLength + scale == 0) {
          clearLeds();
        }
      }

      digitalWrite(ledMode, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Afstand in cm");
      lcd.setCursor(0, 1);
      lcd.print(distanceCm);
    }
  }
  optionSelected = false;
  rotaryPosition = 0;
  clearLeds();
  startMenu();
}

void instellenHoek() {
  /*max()checks if position is greater then 0. If not it returns 0.
    min()checks if position is smaller then 180. If not it returns 180.*/
  rotaryPosition = min(180, max(0, rotaryPosition));

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Instellen van de hoek");
  lcd.setCursor(6, 1);
  lcd.print(rotaryPosition);

  while (!optionSelected) {
    checkButton();
    scrollMenu("Instellen van de hoek ",0);
    if (rotaryTurned()) {
      /*max()checks if position is greater then 0. If not it returns 0.
        min()checks if position is smaller then 100. If not it returns 100.*/
      rotaryPosition = min(180, max(0, rotaryPosition));
      lcd.setCursor(6, 1);
      lcd.print("   ");
      lcd.setCursor(6, 1);
      lcd.print(rotaryPosition);
    }
  }

  if (optionSelected) {
    optionSelected = false;
    hoek = rotaryPosition;
    EEPROM.update(Ehoek, hoek);
    rotaryPosition = 0;
    hoekenMeten();
  }

}

void hoekActiveren() {
  //hoek = map(hoek, 0, 180, 0, 180);
  //Serial.println(hoek);
  servo.write(hoek);
  //Serial.println(servo.read());
  rotaryPosition = 0;
  hoekenMeten();
}

void scrollMenu(String options, int place) {
  static int counter = 0;
  static unsigned long lastScroll = 0;
  unsigned long scrollTime = millis();

  if (scrollTime - lastScroll > 800) {
    static String prevOption = "";
    int rotaryLength = options.length();

    if (rotaryLength > 16-place) {
      if (prevOption.equals(options)) {
        if (rotaryLength - counter + 1 > 16-place) {
          lcd.setCursor(place, 0);
          lcd.print("                ");
          lcd.setCursor(place, 0);
          for (int i = 0; i < options.length(); i++) {
            lcd.print(options[counter + i]);
          }
          counter++;
        }
        else {
          counter = 0;
        }
      }
      else {
        counter = 0;
      }
    }
    prevOption = options;
    lastScroll = scrollTime;
  }
}

void scrollScreen(String line1, String line2){
  static int counter2 = 0;
  static unsigned long lastScreen = 0;
  unsigned long screenTime = millis();
  
  int maxLength = max(line1.length(), line2.length());
  
  if(screenTime - lastScreen > 800) {
     if(maxLength - counter2 + 1 > 16){
      lcd.scrollDisplayLeft();
      counter2++;
     }
     else {
      for(int i=0; i<counter2; i++){
        lcd.scrollDisplayRight();
      }
      counter2 = 0;
    }
   lastScreen = screenTime;
 }
}

bool rotaryTurned() {
  if (rotaryPosition != prevPosition) {
    prevPosition = rotaryPosition ;
    return true;
  }
  else {
    return false;
  }
}

void checkButton() {
  if (digitalRead(buttonPin)) {
    optionSelected = true;
    delay(200);
  }
}

void updatePosition() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  // checks if time difference is greater then 5ms
  if (interruptTime - lastInterruptTime > 5) {
    if (digitalRead(pinB) == LOW)
    {
      rotaryPosition-- ;
    }
    else if (digitalRead(pinB) == HIGH) {
      rotaryPosition++ ;
    }
  }
  lastInterruptTime = interruptTime;
}
