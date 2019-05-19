static int leds[] = {A0,A1,A2,A3,A4,A5};
static int button1 = 2;
static int button2 = 3;
int i = 0;
int randomNr = 0;
int moleNumber = 0;
int tries = 0;
int speelSnelheid = 2000;
volatile bool delayComplete = true;
volatile bool newRandomNumber = false;
volatile bool printRand = false;
volatile bool randLight = false;
volatile bool checkAns = false;
volatile bool newRound = false;
volatile bool stopGame = false;
volatile bool reset = false;



void setup() {
  // put your setup code here, to run once:
    for(i=0;i<6;i++){
      pinMode(leds[i], OUTPUT);
    }
    i = 0;
    
    attachInterrupt(digitalPinToInterrupt(button1), Knop1, RISING);
    attachInterrupt(digitalPinToInterrupt(button2), Start, RISING);

    Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!delayComplete){
    delay(30);
    delayComplete = true;
  }

  if(reset){
    newRound = false;
    i = 0;
    randomNr = 0;
    moleNumber = 0;
    tries = 0;
    speelSnelheid = 2000;
    delayComplete = true;
    newRandomNumber = false;
    printRand = false;
    randLight = false;
    checkAns = false;
    stopGame = false;
    reset = false;
    Serial.println("Spel Onderbroken");
    
    
  }

  if(newRandomNumber){
    randomNr = random(1, 7);
    if(printRand){
        Serial.println(randomNr);
        moleNumber = randomNr;
        printRand = false;
        newRandomNumber = false;
        if(newRound){
          newRandomNumber = true;
          randLight = true;
        }
      }
    if(randLight){
        digitalWrite(leds[randomNr-1], HIGH);
        delay(speelSnelheid);
        digitalWrite(leds[randomNr-1], LOW);
        if(checkAns){
          if(moleNumber == randomNr){
            Serial.println("Raak!!");
            Serial.print("Geraakt in ");
            Serial.print(tries);
            Serial.println(" pogingen!");
            tries = 0;
            speelSnelheid = speelSnelheid*0.8;
            checkAns = false;
            
            newRandomNumber = true;
            printRand = true;
            newRound = true;
          }
          else{
            Serial.println("Fout gemikt");
            tries++;
            checkAns = false;
          }
        }
      }
  }
}

void Knop1(){
    if(randLight){
      checkAns = true;
    }
    else{
      newRandomNumber = true;
      printRand = true;
    }
}

void Start(){
  if(!stopGame){
    newRandomNumber = true;
    randLight = true;
    stopGame = true;
  }
  else{
    reset = true; 
   }
}
