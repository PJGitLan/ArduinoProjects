#include <LiquidCrystal.h> //Lcd library

//Lcd pins
const int rs = 12, en = 11, d4 = 5, d5 = 6, d6 = 7, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Rotary pins
const int pinA = 2;
const int pinB = 4;

//button pin
const int buttonPin = 9;

//Led pins
const int leds[] = {A3,A4,A5,A6,A7};

//Variable that's updated by the ISR "updatePosition()" 
volatile int rotaryPosition, prevPosition = 0;

//flags
bool optionSelected = false;

String serialAns = "NaN";

int i = 0;
int optionsSize = 0;
int currentMenu = 0;

//==============================================================================================//
//Setup      Setup      Setup      Setup      Setup      Setup      Setup      Setup      Setup //
//==============================================================================================//

void setup() {
  Serial.begin(9600);
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
//==============================================================================================//
//Loop      Loop      Loop      Loop      Loop      Loop      Loop      Loop      Loop      Loop//
//==============================================================================================//
void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(buttonPin)){
    optionSelected = true;
    delay(200);
  }
  
  if (rotaryPosition != prevPosition || optionSelected) {
    for(i=0;i<5;i++){
      digitalWrite(leds[i],LOW);  
    }
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
//          case 4:
//            toggleColorMenu(selectedLed);
//            break;
          default:
            startMenu();
            break;
        }
    
    // Keep track of this new value
    prevPosition = rotaryPosition ;
  }
}

//==============================================================================================//
//Menu Printer      Menu Printer      Menu Printer      Menu Printer      Menu Printer      Menu//
//==============================================================================================//

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

//==============================================================================================//
//Start Menu      Start Menu      Start Menu      Start Menu      Start Menu      Start Menu    //
//==============================================================================================//

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

//==============================================================================================//
//Afstand meten      Afstand meten      Afstand meten      Afstand meten      Afstand meten     //
//==============================================================================================//

  void afstandMeten(){
    if(optionSelected){
      optionSelected = false;
      int temp = rotaryPosition;
      rotaryPosition = 0;
     
      switch(temp){
        case 0:
          minimumMeetwaarde();
          break;
//        case 1:
//          hoekenMeten();
//          break;
      }
      return;
      
    }
    Serial.println("in afstand 2");
    digitalWrite(leds[0],HIGH);
    currentMenu=1;
    optionsSize = 5;
    String options[optionsSize] = {"minimum meetwaarde instellen","maximum meetwaarde instellen","bekijken verdeling over de led's","meten activeren","terug naar het hoofdmenu"};
    printMenu(options, optionsSize); 
    
  }
//==============================================================================================//
//Hoeken meten      Hoeken meten      Hoeken meten      Hoeken meten      Hoeken meten      Hoek//
//==============================================================================================//

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

//==============================================================================================//
//Minimum meetwaarde      Minimum meetwaarde      Minimum meetwaarde      Minimum meetwaarde    //
//==============================================================================================//

  void minimumMeetwaarde(){
    if(optionSelected){
      
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

     serialAns = "NaN";
     if (Serial.available() > 0){
      serialAns = Serial.readString();
      if(isNaN(serialAns))
      Serial.flush();
     }
  }

//==============================================================================================//
//ISR: update position      ISR: update position      ISR: update position      ISR: update posi//   
//==============================================================================================//
  
void updatePosition(){
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();

  // If interrupts come faster than 5ms, assume it's a bounce and ignore
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
