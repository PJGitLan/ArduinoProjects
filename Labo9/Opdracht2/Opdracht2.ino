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
  //lcd.autoscroll();
 
}

void loop() {
  // put your main code here, to run repeatedly:
  if(menuChooser){
    switch(currentMenu){
      case 0:
        startMenu();
        break;
      case 1:
        individueelMenu();
        break;
      case 2:
        toggleNrMenu(selectedLed);
        break;
      case 3:
        kleurMenu();
        break;
      case 4:
        toggleColorMenu(selectedLed);
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
    optieGeselecteerd = true;
    menuChooser = true;
    debounceK2 = true; 
  }
}

void printMenu(String opties[],int aantalOpties){
    
  dePlaats = plaats%aantalOpties;
  //Serial.println(dePlaats);
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
      
      plaats = 0;
      optieGeselecteerd = false;
      menuChooser = true;
      
      switch(dePlaats){
      case 0:
        currentMenu = 1;
        individueelMenu();
        break;
      case -1:
        currentMenu = 3;
        kleurMenu();
        break;
      default:
        currentMenu = 0;
        break;
      }
      return;
    }
    int aantalOpties = 2;
    String opties[aantalOpties] = {"individuele led","kleur"};
    printMenu(opties, aantalOpties); 
  }

void individueelMenu(){
    if(optieGeselecteerd){
      plaats = 0;
      optieGeselecteerd = false;
      menuChooser = true;
      if(dePlaats<4&&dePlaats>=0){
        optieGeselecteerd = false;
        selectedLed = led[dePlaats];
        currentMenu = 2;
        toggleNrMenu(selectedLed);
      }
      else if(dePlaats==-1){
        optieGeselecteerd = false;
        currentMenu = 0;
        startMenu();
      }
      return;
    }
   
    int aantalOpties = 5;
    String opties[aantalOpties] = {"1","2", "3", "4", "Terug naar het hoofdmenu"};
    printMenu(opties, aantalOpties); 
}

void toggleNrMenu(int selectedLed){
   
   if(optieGeselecteerd){
      
      plaats = 0; 
      optieGeselecteerd = false;
      Serial.println(dePlaats);
      switch(dePlaats){
      case 0:
        digitalWrite(selectedLed,HIGH);
        currentMenu = 0;
        startMenu();
        break;
      case -1:
        digitalWrite(selectedLed,LOW);
        currentMenu = 0;
        startMenu();
        break;
      default:
        currentMenu = 0;
        startMenu();
        break;
      }
      return;
    }
    
   int aantalOpties = 2;
   String opties[aantalOpties] = {"Aan","Uit"};
   printMenu(opties, aantalOpties); 
}

void kleurMenu(){
    if(optieGeselecteerd){
      
      plaats = 0;
      optieGeselecteerd = false;
      menuChooser = true;
      
      switch(dePlaats){
      case 0:
        Serial.println("hier?");
        currentMenu = 4;
        selectedLed = led[0];
        toggleColorMenu(selectedLed);
        Serial.println("hier?");
        break;
      case 1:
        currentMenu = 4;
        selectedLed = led[2];
        toggleColorMenu(selectedLed);
        break;
      default:
        currentMenu = 0;
        startMenu();
        break;
      }
      return;
    }
    int aantalOpties = 3;
    String opties[aantalOpties] = {"rood","groen","Terug naar het hoofdmenu"};
    printMenu(opties, aantalOpties); 
  }

 void toggleColorMenu(int led){
    if(optieGeselecteerd){
      
      plaats = 0;
      optieGeselecteerd = false;
      menuChooser = true;
      
      switch(dePlaats){
      case 0:
        Serial.println("ok");
        digitalWrite(led,HIGH);
        digitalWrite(led+1,HIGH);
        currentMenu = 0;
        startMenu();
        break;
      case -1:
        digitalWrite(led,LOW);
        digitalWrite(led+1,LOW);
        currentMenu = 4;
        currentMenu = 0;
        startMenu();
        break;
      default:
        currentMenu = 0;
        startMenu();
        break;
      }
      return;
    }
    int aantalOpties = 2;
    String opties[aantalOpties] = {"Aan","Uit"};
    printMenu(opties, aantalOpties); 
  }
