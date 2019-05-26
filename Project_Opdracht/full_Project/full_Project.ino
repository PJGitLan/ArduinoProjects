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
int currentMenu = 0;
int hoek = EEPROM.read(Ehoek);

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
  checkButton();
 
  if (rotaryPosition != prevPosition || optionSelected) {
    clearLeds();
    switch(currentMenu){
          case 0:
            startMenu();
            break;
          case 1:
            afstandMeten();
            break;
          case 2:
            hoekenMeten();
            break;
          case 3:
            minimumMeetwaarde();
            break;
          case 4:
            maximumMeetwaarde();
            break;
          case 5:
            verdelingLeds();
            break;
          case 6:
            metenActiveren();
            break;
          case 7:
            instellenHoek();
            break;
          default:
            startMenu();
            break;
        }
    
    // Keep track of this new value
    prevPosition = rotaryPosition ;
  }
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
        return;
      }
          
    currentMenu=0;
    optionsSize = 2;
    String options[optionsSize] = {"afstand meten","hoeken meten"};
    printMenu(options, optionsSize); 
  }

void afstandMeten(){
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
          startMenu();
          break;
      }
      return;
      
    }
    digitalWrite(ledMode,HIGH); 
    currentMenu=1;
    optionsSize = 5;
    String options[optionsSize] = {"minimum meetwaarde instellen","maximum meetwaarde instellen","bekijken verdeling over de led's","meten activeren","terug naar het hoofdmenu"};
    printMenu(options, optionsSize); 
    
  }

void hoekenMeten(){
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
        return;
      }
     
    currentMenu=2;
    optionsSize = 3;
    String options[optionsSize] = {"instellen van de hoek","hoek activeren","terug naar het hoofdmenu"};
    printMenu(options, optionsSize); 
  }

void minimumMeetwaarde(){
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
      return;
    }

    digitalWrite(ledMode,HIGH);
    currentMenu=3;
    //max()checks if position is greater then 0. If not it returns 0.
    //min()checks if position is smaller then 100. If not it returns 100.
    rotaryPosition = min(400, max(2, rotaryPosition));
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Minimum meetwaarde:");
    lcd.setCursor(6,1);
    lcd.print(rotaryPosition);
}

void maximumMeetwaarde(){
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
      return;
    }
    digitalWrite(ledMode,HIGH);
    currentMenu=4;
    //max()checks if position is greater then 0. If not it returns 0.
    //min()checks if position is smaller then 100. If not it returns 100.
    rotaryPosition = min(400, max(2, rotaryPosition));
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Maximum meetwaarde:");
    lcd.setCursor(6,1);
    lcd.print(rotaryPosition);
}

void calculatingScale(){
  scale = (maximumLength-minimumLength)/4;
}

void verdelingLeds(){
  if(optionSelected){
      optionSelected = false;
      rotaryPosition = 0;
      afstandMeten();
      return;
  }
  digitalWrite(ledMode,HIGH);
  currentMenu=5;
  calculatingScale();
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
}

void metenActiveren(){
  while(!optionSelected){
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
     checkButton();
  }
    optionSelected = false;
    rotaryPosition = 0;
    clearLeds();
    startMenu();
}

void instellenHoek(){
  if(optionSelected){
    optionSelected = false;
    hoek = rotaryPosition;
    EEPROM.update(Ehoek, hoek);
    rotaryPosition = 0;
    hoekenMeten();
    return;
   }
    
    currentMenu=7;
    //max()checks if position is greater then 0. If not it returns 0.
    //min()checks if position is smaller then 100. If not it returns 100.
    rotaryPosition = min(180, max(0, rotaryPosition));
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Intellen van de hoek");
    lcd.setCursor(6,1);
    lcd.print(rotaryPosition);
}

void hoekActiveren(){
  
  //hoek = map(hoek, 0, 180, 0, 180);
  //Serial.println(hoek);
  servo.write(hoek);
  //Serial.println(servo.read());
  rotaryPosition = 0;
  hoekenMeten();
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
