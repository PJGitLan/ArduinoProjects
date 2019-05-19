#include <EEPROM.h>

int counter = 0;
int address = 0;

void setup() {
  // put your setup code here, to run once: 
  Serial.begin(9600);
  counter = EEPROM.read(address);
  counter++;
  
  if(counter > 255){
    counter = 0;
  }
  
  EEPROM.write(address, counter);
  Serial.print("Counter: ");
  //Serial.println(counter);
  Serial.println(EEPROM.read(address));
}

void loop(){}
//Bij het resetten van de arduino stijgt de teller met 1.
//Als De arduino wordt uitgetrokken stijgt de teller met 2 omdat dan ook het seriële diplay opnieuw moet worden aangezet.
//Dit runt de setup() functie ook opnieuw waardoor de teller een keer extra stijgt.
