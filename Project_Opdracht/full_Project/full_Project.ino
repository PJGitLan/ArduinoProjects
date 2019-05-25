#include <LiquidCrystal.h> //Lcd library
#include <NewPing.h> //Ultrasone sensor library

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

//Sonar pins &setup
const int echoPin = 3;
const int trigPin = 5;
int maxDistance = 200;
NewPing sonar(trigPin, echoPin, maxDistance);

//Variable that's updated by the ISR "updatePosition()" 
volatile int rotaryPosition, prevPosition = 0;

//flags
bool optionSelected = false;

//String serialAns = "NaN";

int i = 0;
int optionsSize = 0;
int currentMenu = 0;
int minimumLength = 2;
int maximumLength = 300;

unsigned int previousDistance = 0;

float scale = 0;

//^^^ Global vars etc ^^^

void setup() {
  Serial.begin(9600);
  //Serial.setTimeout(100);
  lcd.begin(16,2);

  pinMode(pinA, INPUT_PULLUP);
  pinMode(pinB, INPUT_PULLUP);
  pinMode(buttonPin,INPUT);
  
  for(i=0;i<5;i++){
    pinMode(leds[i],OUTPUT);
  }
  
  attachInterrupt(digitalPinToInterrupt(pinA), updatePosition, LOW);//FALLING gives errors

  startMenu();
}

void loop() {
  
  // put your main code here, to run repeatedly:
  checkButton();
 
  if (rotaryPosition != prevPosition || optionSelected) {
    //clearLeds();
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
          Serial.println("frfgrtg");
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
          Serial.println(currentMenu);
          break;
      }
      return;
      
    }
    digitalWrite(leds[0],HIGH); 
    currentMenu=1;
    optionsSize = 5;
    String options[optionsSize] = {"minimum meetwaarde instellen","maximum meetwaarde instellen","bekijken verdeling over de led's","meten activeren","terug naar het hoofdmenu"};
    printMenu(options, optionsSize); 
    
  }

void hoekenMeten(){
//    if(optionSelected){
//      optionSelected = false;
//      int temp = rotaryPosition;
//      rotaryPosition = 0;
//      
//      switch(temp){
//        case 0:
//          afstandMeten();
//          break;
////        case 1:
////          hoekenMeten();
////          break;
//        }
//        return;
//      }
      
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
        calculatingScale();
        rotaryPosition = 0;
        afstandMeten();
      }
      else{
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
        calculatingScale();
        rotaryPosition = 0;
        afstandMeten();
      }
      else{
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
  currentMenu=5;
  calculatingScale();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("L1:");
  lcd.print(minimumLength+scale*0,1);
  lcd.print(" L2:");
  lcd.print(minimumLength+scale,1);
  lcd.print(" L3:");
  lcd.print(minimumLength+scale*2,1);
  lcd.setCursor(0,1);
  lcd.print("L4:");
  lcd.print(minimumLength+scale*3,1);
  lcd.print(" L5:");
  lcd.print(minimumLength+scale*4,1);
}

void metenActiveren(){
  while(!optionSelected){
    optionSelected = false;
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
      }
      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Afstand in cm");
      lcd.setCursor(0,1);
      lcd.print(distanceCm);
    }
     
     checkButton();
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
