#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int led[4]= {A0,A1,A2,A3};
const int knop1= 2;
const int knop2= 3;

int plaats = 0;
int dePlaats = 0;
int i = 0;
int selectedLed = 0;
int currentMenu = 0; //startMenu() = 0
                     //individueelMenu() = 1
                     //toggleNrMenu() = 2
                     //kleurMenu() = 3
                     //toggleColorMenu() = 4

bool menuChooser = true;
bool optieGeselecteerd = false;
bool debounceK1 = false;
bool debounceK2 = false;

void setup() {
  // put your setup code here, to run once:
  for(i = 0; i<4;i++){
    pinMode(led[i],OUTPUT);
  }

  attachInterrupt(digitalPinToInterrupt(knop1),onKnop1, FALLING);
  attachInterrupt(digitalPinToInterrupt(knop2),onKnop2, FALLING);
  Serial.begin(9600);
  lcd.begin(16,2);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(menuChooser){
    Serial.println("FLAG 7");
    switch(currentMenu){
      case 0:
        startMenu();
        break;
      case 1:
      Serial.print("FLAG 3");
      Serial.print(optieGeselecteerd);
        individueelMenu();
        Serial.print("FLAG 4");
        break;
      case 2:
        //toggleNrMenu(selectedLed);
        break;
      case 3:
        //kleurMenu();
        break;
      case 4:
       // toggleColorMenu();
        break;
      default:
        startMenu();
        break;
    }
    menuChooser = false;
  }
  
  if(debounceK1||debounceK2){
    delay(200);
    debounceK1 = false;
    debounceK2 = false;
    }
}

void onKnop1(){
  if(!debounceK1){
    plaats++;
    menuChooser = true;
    debounceK1 = true; 
  }
} 

void onKnop2(){
  if(!debounceK2){
    menuChooser = true;
    optieGeselecteerd = true;
    debounceK2 = true; 
  }
}

void printMenu(String opties[],int aantalOpties){
    
  dePlaats = plaats%aantalOpties;
  Serial.println("in print");
  Serial.println(dePlaats);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(">");
  
  lcd.setCursor(1,0);
  lcd.print(opties[dePlaats]);
  lcd.setCursor(1,1);
  if(dePlaats+1>=aantalOpties){
    dePlaats = -1;
  }
  lcd.print(opties[dePlaats+1]);
}

void startMenu(){
    if(optieGeselecteerd){
      Serial.println("FLAG 5");
      plaats = 0;
      optieGeselecteerd = false;
      menuChooser = true;
      
      switch(dePlaats){
      case 0:
      Serial.println("FLAG 6");
        currentMenu = 1;
        
        break;
      case 1:
        //currentMenu = 3;
        break;
      default:
        currentMenu = 0;
        break;
      }
    }
    else{
      Serial.println("did not break");
      int aantalOpties = 2;
      String opties[aantalOpties] = {"individuele led","kleur"};
      printMenu(opties, aantalOpties);
    } 
  }

void individueelMenu(){
    if(optieGeselecteerd){
      Serial.println("FLAG 1");
      plaats = 0;
      optieGeselecteerd = false;
      menuChooser = true;
      selectedLed = led[dePlaats];
      currentMenu = 2;
      return;
    }
   Serial.println("FLAG 2");
    int aantalOpties = 5;
    String opties[aantalOpties] = {"1","2", "3", "4", "Terug naar het hoofdmenu"};
    printMenu(opties, aantalOpties); 
}
