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

//Rotary pins
const int pinA = 2;
const int pinB = 4;

//button pin
const int buttonPin = 13;

//Led pins
const int leds[] = {A0,A1,A2,A3,A4};
const int ledMode = A5;

//Sonar pins &setup
const int echoPin = 3;
const int trigPin = 5;
int maxDistance = 400; //Shorter maxDistance makes quicker pings/measurements
NewPing sonar(trigPin, echoPin, maxDistance);

//Servo setup
Servo servo;

//Variable that's updated by the ISR "updatePosition()" 
volatile int rotaryPosition, prevPosition = 0;

//flags
bool optionSelected = false;

//variables
int i = 0;
int optionsSize = 0;
int hoek = EEPROM.read(Ehoek);
int counter = 0;

unsigned int previousDistance = 0;

float scale = 0;
float minimumLength = EEPROM.read(EminLength);
float maximumLength = EEPROM.read(EmaxLength);

//^^^ Global vars etc ^^^

void setup() {
  //Serial.begin(9600);
  //Serial.setTimeout(100);
  lcd.begin(16,2);
  servo.attach(6);
  
  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(buttonPin,INPUT);
  
  for(i=0;i<5;i++){
    pinMode(leds[i],OUTPUT);
  }
  pinMode(ledMode, OUTPUT);
  
  attachInterrupt(digitalPinToInterrupt(pinA), updatePosition, LOW);//FALLING gives errors
  calculatingScale(); 
  startMenu();
}

void loop() {
  // put your main code here, to run repeatedly:
}

void clearLeds(){
  for(i=0;i<5;i++){
      digitalWrite(leds[i],LOW);  
    }
  digitalWrite(ledMode,LOW);
}

void printMenu(String options[],int optionsSize){
  if(rotaryPosition < 0){
    rotaryPosition = optionsSize-1;
  }
  else{
    rotaryPosition = rotaryPosition%optionsSize;
  }
  
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(">");
  //Serial.println(rotaryPosition);
  lcd.setCursor(1,0);
  lcd.print(options[rotaryPosition]);
  lcd.setCursor(1,1);
  if(rotaryPosition+1>=optionsSize){
    lcd.print(options[0]);
  }
  else{
    lcd.print(options[rotaryPosition+1]);
  }
}

void startMenu(){
  optionsSize = 2;
  String options[optionsSize] = {"afstand meten","hoeken meten"};
  printMenu(options, optionsSize);
      
  while(!optionSelected){
    checkButton();
    if(rotaryTurned()){
      //optionsSize = 2;
      //String options[optionsSize] = {"afstand meten","hoeken meten"};
      printMenu(options, optionsSize);
    }
  }
  
  if(optionSelected){
    optionSelected = false;  
    int temp = rotaryPosition;
    rotaryPosition = 0;
    switch(temp){
      case 0:
        afstandMeten();
        break;
      case 1:
        hoekenMeten();
        break;
      }
    }
  }

void afstandMeten(){
  digitalWrite(ledMode,HIGH); 
  optionsSize = 5;
  String options[optionsSize] = {"minimum meetwaarde instellen","maximum meetwaarde instellen","bekijken verdeling over de led's","meten activeren","terug naar het hoofdmenu"};
  printMenu(options, optionsSize);

  while(!optionSelected){
    checkButton();
    scrollMenu(options, optionsSize);
    if(rotaryTurned()){
      //optionsSize = 5;
      //String options[optionsSize] = {"minimum meetwaarde instellen","maximum meetwaarde instellen","bekijken verdeling over de led's","meten activeren","terug naar het hoofdmenu"};
      printMenu(options, optionsSize);
    }
  }
  
  if(optionSelected){
    optionSelected = false;
    int temp = rotaryPosition;
    rotaryPosition = 0;
    switch(temp){
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

void hoekenMeten(){
    optionsSize = 3;
    String options[optionsSize] = {"instellen van de hoek","hoek activeren","terug naar het hoofdmenu"};
    printMenu(options, optionsSize);

    while(!optionSelected){
      checkButton();
      if(rotaryTurned()){
        printMenu(options, optionsSize);
      }
    }

    if(optionSelected){
      optionSelected = false;
      int temp = rotaryPosition;
      rotaryPosition = 0;
      
      switch(temp){
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

void minimumMeetwaarde(){

    digitalWrite(ledMode,HIGH);
    /*max()checks if position is greater then 0. If not it returns 0.
    min()checks if position is smaller then 100. If not it returns 100.*/
    rotaryPosition = min(400, max(2, rotaryPosition));
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Minimum meetwaarde:");
    lcd.setCursor(6,1);
    lcd.print(rotaryPosition);

    while(!optionSelected){
      checkButton();
      if(rotaryTurned()){
        /*max()checks if position is greater then 0. If not it returns 0.
        min()checks if position is smaller then 100. If not it returns 100.*/
        rotaryPosition = min(400, max(2, rotaryPosition));
        lcd.setCursor(6,1);
        lcd.print("   ");
        lcd.setCursor(6,1);
        lcd.print(rotaryPosition);
      }
    }

    if(optionSelected){
      optionSelected = false;
      if(rotaryPosition<maximumLength){
        minimumLength = rotaryPosition;
        EEPROM.update(EminLength, minimumLength);
        calculatingScale();
        rotaryPosition = 0;
        afstandMeten();
      }
      else{
        digitalWrite(ledMode,HIGH);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Error:");
        lcd.setCursor(0,1);
        lcd.print("minimum>maximum");
        delay(3000);
        minimumMeetwaarde();
      }
    }
}

void maximumMeetwaarde(){
  
    digitalWrite(ledMode,HIGH);
    /*max()checks if position is greater then 0. If not it returns 0.
    min()checks if position is smaller then 100. If not it returns 100.*/
    rotaryPosition = min(400, max(2, rotaryPosition));
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Maximum meetwaarde:");
    lcd.setCursor(6,1);
    lcd.print(rotaryPosition);

    while(!optionSelected){
      checkButton();
      if(rotaryTurned()){
        /*max()checks if position is greater then 0. If not it returns 0.
        min()checks if position is smaller then 100. If not it returns 100.*/
        rotaryPosition = min(400, max(2, rotaryPosition));
        lcd.setCursor(6,1);
        lcd.print("   ");
        lcd.setCursor(6,1);
        lcd.print(rotaryPosition);
      }
    }
    
   if(optionSelected){
    optionSelected = false;
      if(rotaryPosition>minimumLength){
        maximumLength = rotaryPosition;
        EEPROM.update(EmaxLength, maximumLength);
        calculatingScale();
        rotaryPosition = 0;
        afstandMeten();
      }
      else{
        digitalWrite(ledMode,HIGH);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Error:");
        lcd.setCursor(0,1);
        lcd.print("maximum<minimum");
        delay(3000);
        maximumMeetwaarde();
      }
    }
}

void calculatingScale(){
  scale = (maximumLength-minimumLength)/4;
}

void verdelingLeds(){
  digitalWrite(ledMode,HIGH);
  //calculatingScale();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("L1:");
  lcd.print(minimumLength+scale*0,2);
  lcd.print(" L2:");
  lcd.print(minimumLength+scale,2);
  lcd.print(" L3:");
  lcd.print(minimumLength+scale*2,2);
  lcd.setCursor(0,1);
  lcd.print("L4:");
  lcd.print(minimumLength+scale*3,2);
  lcd.print(" L5:");
  lcd.print(minimumLength+scale*4,2);
  
  while(!optionSelected){
    checkButton();
  }
  
  if(optionSelected){
      optionSelected = false;
      rotaryPosition = 0;
      afstandMeten();
  }
}

void metenActiveren(){
  while(!optionSelected){
    checkButton();
    //delay(35); 
    unsigned int echoTime = sonar.ping_median();
    unsigned int distanceCm = sonar.convert_cm(echoTime);
    
    if(distanceCm != previousDistance){
      previousDistance =  distanceCm;
      
      for(int i=0;i<5;i++){
        if(minimumLength+scale*i<=distanceCm){
          clearLeds();
          digitalWrite(leds[i],HIGH);
        }
        if(minimumLength+scale==0){
          clearLeds();
        }
      }
      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Afstand in cm");
      lcd.setCursor(0,1);
      lcd.print(distanceCm);
    }
  }
    optionSelected = false;
    rotaryPosition = 0;
    clearLeds();
    startMenu();
}

void instellenHoek(){
  /*max()checks if position is greater then 0. If not it returns 0.
    min()checks if position is smaller then 100. If not it returns 100.*/
  rotaryPosition = min(180, max(0, rotaryPosition));
    
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Intellen van de hoek");
  lcd.setCursor(6,1);
  lcd.print(rotaryPosition);
  
  while(!optionSelected){
    checkButton();
      if(rotaryTurned()){
        /*max()checks if position is greater then 0. If not it returns 0.
        min()checks if position is smaller then 100. If not it returns 100.*/
        rotaryPosition = min(180, max(0, rotaryPosition));
        lcd.setCursor(6,1);
        lcd.print("   ");
        lcd.setCursor(6,1);
        lcd.print(rotaryPosition);
      }
    }
  
  if(optionSelected){
    optionSelected = false;
    hoek = rotaryPosition;
    EEPROM.update(Ehoek, hoek);
    rotaryPosition = 0;
    hoekenMeten();
   }
    
}

void hoekActiveren(){  
  //hoek = map(hoek, 0, 180, 0, 180);
  //Serial.println(hoek);
  servo.write(hoek);
  //Serial.println(servo.read());
  rotaryPosition = 0;
  hoekenMeten();
}

void scrollMenu(String options[], int optionsSize){

//if(options[rotaryPosition].length()>15){
//if(options[rotaryPosition].length()-15<counter){
  
  if(20>15){
    if(counter<5){
      lcd.setCursor(1,0);
      lcd.print("                ");
      lcd.setCursor(1,0);
      for(int i = 0; i < options[rotaryPosition].length();i++){
        lcd.print(options[rotaryPosition][counter+i]);
      }
      counter++;
      delay(500); 
    }
  }
}

bool rotaryTurned(){
   if (rotaryPosition != prevPosition){
      prevPosition = rotaryPosition ;
      return true;
   }
   else{
    return false;
   }
}

void checkButton(){
  if(digitalRead(buttonPin)){
    optionSelected = true;
    delay(200);
  }
}
  
void updatePosition(){
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  // checks time against previous interruptTime
  if (interruptTime - lastInterruptTime > 5) {
    if (digitalRead(pinB) == LOW)
    {
      rotaryPosition-- ;
    }
    else if(digitalRead(pinB) == HIGH){
      rotaryPosition++ ;
    }
  }
  // Keep track of when we were here last (no more than every 5ms)
  lastInterruptTime = interruptTime;
}
