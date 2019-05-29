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
int hoek = EEPROM.read(Ehoek);

float scale = 0;
float minimumLength = EEPROM.read(EminLength);
float maximumLength = EEPROM.read(EmaxLength);

//^^^ Global vars etc ^^^

void setup() {
  //Serial.begin(9600);
  //Serial.setTimeout(100);
  lcd.begin(16, 2); //Initiates 16x2 lcd screen
  servo.attach(servoPin,800, 2800); //Attaches servo to right pin + added right pulses for min(0°=~1ms) and max(180°=~2ms) for SG90 servo (info from datasheet)
                                    //After testing 0.8ms and 2.8ms are more accurate values    
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

void printMenu(String options[], int optionsSize) { //prints right options on lcd 
  
  if (rotaryPosition < 0) { //scrolling down on first option selects last option
    rotaryPosition = optionsSize - 1;
  }
  else {
    rotaryPosition = rotaryPosition % optionsSize; //Makes sure rotaryPostion doesn't becomes greater then optionsSize
  }

  //Prints options to the lcd screen
  //Serial.println(options[rotaryPosition]);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(">");
  lcd.setCursor(1, 0);
  lcd.print(options[rotaryPosition]);
  lcd.setCursor(1, 1);
  if (rotaryPosition + 1 >= optionsSize) { //Latest option will be followed up by first option
    lcd.print(options[0]);
  }
  else {
    lcd.print(options[rotaryPosition + 1]); //shows next available option
  }
}

void startMenu() {
  static const int optionsSize = 2; //static and constant to avoid memory leak
  static const String options[optionsSize] = {"afstand meten", "hoeken meten"};
  printMenu(options, optionsSize);

  menuLoop(options, optionsSize);

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
  static const int optionsSize = 5; //static and constant to avoid memory leak
  static const String options[optionsSize] = {"minimum meetwaarde instellen  ", "maximum meetwaarde instellen  ", "bekijken verdeling over de led's  ", "meten activeren", "terug naar het hoofdmenu  "};
  printMenu(options, optionsSize);

  menuLoop(options, optionsSize);

  if (optionSelected) { //Select next menu
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
        return; //uses return instead of startmenu() to avoid memory leak
        //startMenu();
        break;
    }
  }
}

void hoekenMeten() {
  static const int optionsSize = 3; //static and constant to avoid memory leak
  static const String options[optionsSize] = {"instellen van de hoek  ", "hoek activeren", "terug naar het hoofdmenu  "};
  printMenu(options, optionsSize);

  menuLoop(options, optionsSize);

  if (optionSelected) { //Select next menu
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
        return; //uses return instead of startmenu() to avoid memory leak
        //startMenu();
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

  while (!optionSelected) {//checks for inputs and updates screen
    checkButton();
    scrollMenu("Minimum meetwaarde: ",0);
    if (rotaryTurned()) {
      /*max()checks if position is greater then 2. If not it returns 2.
        min()checks if position is smaller then 400. If not it returns 400.*/
      rotaryPosition = min(400, max(2, rotaryPosition));
      lcd.setCursor(6, 1);
      lcd.print("   ");
      lcd.setCursor(6, 1);
      lcd.print(rotaryPosition);
    }
  }

  if (optionSelected) { //Saves minimumlength
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

  while (!optionSelected) { //checks for inputs and updates screen
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

  if (optionSelected) { //Saves maximumlength
    optionSelected = false;
    if (rotaryPosition > minimumLength) {
      maximumLength = rotaryPosition;
      EEPROM.update(EmaxLength, maximumLength);
      calculatingScale();
      rotaryPosition = 0;
      afstandMeten();
    }
    else { //Error message 
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
  static String firstLine = "";
  static String secondLine = "";

  for (int i = 0; i < 2; i++) { //Adds correct value to string "firstLine"
    firstLine = firstLine + "L" + (i + 1) + ":" + (minimumLength + scale * i) + " ";
  }

  for (int i = 2; i < 5; i++) { //Adds correct value to string "secondLine"
    secondLine = secondLine + "L" + (i + 1) + ":" + (minimumLength + scale * i) + " ";
  }
  
  digitalWrite(ledMode, HIGH);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(firstLine);
  lcd.setCursor(0, 1);
  lcd.print(secondLine);
  
  while (!optionSelected) { //checks for button press and scrollsscreen if necessary 
    checkButton();
    scrollScreen(firstLine, secondLine);
  }
  
  if (optionSelected) { //goes back to previous menu
  optionSelected = false;
  rotaryPosition = 0;
  afstandMeten();
}
}

void metenActiveren() {
  while (!optionSelected) { //checks for button press
    checkButton();
    //delay(35);
    static unsigned int previousDistance = 0;
    unsigned int echoTime = sonar.ping_median();  //Measures ping time 5 times en takes median 
    unsigned int distanceCm = sonar.convert_cm(echoTime); //converts ping time into distance

    digitalWrite(ledMode, HIGH);
    //prints initial screen
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Afstand in cm");
      lcd.setCursor(0, 1);
      lcd.print(distanceCm);

    if (distanceCm != previousDistance) { //checks for changes in distance & updates leds accordingly
      previousDistance =  distanceCm;

      for (int i = 0; i < 5; i++) {
        if (minimumLength + scale * i <= distanceCm) {
          clearLeds();
          digitalWrite(leds[i], HIGH);
        }
        if (distanceCm == 0) {
          clearLeds();
        }
      }

      digitalWrite(ledMode, HIGH);
      //updates screen
      //lcd.clear();
      //lcd.setCursor(0, 0);
      //lcd.print("Afstand in cm");
      lcd.setCursor(0, 1);
      lcd.print("                 ");
      lcd.print(distanceCm);
    }
  }
  optionSelected = false;
  rotaryPosition = 0;
  clearLeds();
  return; //uses return instead of startmenu() to avoid memory leak
  //startMenu();
}

void instellenHoek() {
  /*max()checks if position is greater then 0. If not it returns 0.
    min()checks if position is smaller then 180. If not it returns 180.*/
  rotaryPosition = min(180, max(0, rotaryPosition));
  //prints initial screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Instellen van de hoek");
  lcd.setCursor(6, 1);
  lcd.print(rotaryPosition);

  while (!optionSelected) { 
    checkButton();
    scrollMenu("Instellen van de hoek ",0);
    if (rotaryTurned()) { //Updates screen if rotary is turned
      /*max()checks if position is greater then 0. If not it returns 0.
        min()checks if position is smaller then 180. If not it returns 180.*/
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

void menuLoop(String options[], int optionsSize){ //Loop that checks for button presses and rotary changes. Updates screen if needed
  while (!optionSelected) { 
    checkButton();
    scrollMenu(options[rotaryPosition],1);
    if (rotaryTurned()) {
      printMenu(options, optionsSize);
    }
  }
}

void scrollMenu(String options, int place) {
  static int counter = 0;
  static unsigned long lastScroll = 0; 
  unsigned long scrollTime = millis(); //checks current time

  if (scrollTime - lastScroll > 800) { //checks current time against previous time this function is called and checks for a 800ms time difference
    static String prevOption = "";
    int rotaryLength = options.length();

    if (rotaryLength > 16-place) { //checks if screen is larger then lcd screen
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
  
  int maxLength = max(line1.length(), line2.length()); //returns biggest string length
  
  if(screenTime - lastScreen > 800) { //800ms delay
     if(maxLength - counter2 + 1 > 16){ //checks if screen can be scrolled
      lcd.scrollDisplayLeft();
      counter2++;
     }
     else { //resets screenposition and resets counter
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
    delay(500);
  }
}

void updatePosition() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  // checks if time difference is greater then 5ms
  if (interruptTime - lastInterruptTime > 5) {
    if (digitalRead(pinB) == LOW)
    {
      rotaryPosition-- ; //if pinA is LOW and pinB is LOW. Rotary is turnend counterclockwise.  
    }
    else if (digitalRead(pinB) == HIGH) {
      rotaryPosition++ ; //if PinA is LOW and pinB is HIGH. Rotary is turnend clockwise.  
    }
  }
  lastInterruptTime = interruptTime;
}
