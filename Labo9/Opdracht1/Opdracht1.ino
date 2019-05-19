#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
   
  
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0,0);
  lcd.print("Hallo AP");
  delay(1000);
  
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("Hallo AP");
  delay(1000);
  
  lcd.clear();
  lcd.setCursor(4,1);
  lcd.print("Hallo AP");
  delay(1000);
  lcd.clear();
  //lcd.display();
  
  lcd.setCursor(8,1);
  lcd.print("Hallo AP");
  delay(1000);
  lcd.clear();

  lcd.setCursor(8,0);
  lcd.print("Hallo AP");
  delay(1000);
  lcd.clear();

  lcd.setCursor(4,0);
  lcd.print("Hallo AP");
  delay(1000);
  lcd.clear();
  //lcd.blink();
  //lcd.noDisplay();

  //lcd.scrollDisplayLeft();
  
}
