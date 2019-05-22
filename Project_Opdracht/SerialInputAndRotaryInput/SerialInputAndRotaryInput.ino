void minimumMeetwaarde(){
    if(optionSelected){
      if(number>=2&&number<=400&&minimumLength<maximumLength){
        minimumLength = number;
      }
      else{
        minimumLength = rotaryPosition;
      }
      return;
        }
    currentMenu=3;

    //Input via rotary
    //max()checks if position is greater then 0. If not it returns 0.
    //min()checks if position is smaller then 100. If not it returns 100.
    rotaryPosition = min(400, max(2, rotaryPosition));

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Minimum meetwaarde:");
    lcd.setCursor(6,1);
    lcd.print(rotaryPosition);

    //Input via Serial
    if(Serial.available() > 0){
      serialAns = Serial.readString();
      int number;
      //checking if string is int
      int counter = 0;
      for(i=0; i<serialAns.length(); i++){
        if(isDigit(serialAns[i])){
          counter++;
        }
      }

      if(serialAns.length()-1 == counter){
        number = serialAns.toInt();
      }
      else{
        number = -1;
        lcd.setCursor(0,0);
        lcd.print("Minimum meetwaarde:");
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Input is NaN");
      }
    }
  }
