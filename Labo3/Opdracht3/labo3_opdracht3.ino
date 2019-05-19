static int leds[] = {4,5,6};
static int button1 = 2;
static int button2 = 3;
int i = 0;
volatile bool delayComplete = true;

void setup() {
  // put your setup code here, to run once:
    for(i=0;i<3;i++){
      pinMode(leds[i], OUTPUT);
    }
    i = 0;
    
    attachInterrupt(digitalPinToInterrupt(button1), On, RISING);
    attachInterrupt(digitalPinToInterrupt(button2), Off, RISING);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(!delayComplete){
    delay(30);
    delayComplete = true;
  }
}

void On(){
  if(i<=2&&delayComplete){
    delayComplete = false;
    digitalWrite(leds[i],HIGH);
    if(i<=1){
        i++;
      }
    }
  }

  void Off(){
    if(i>=0&&delayComplete){
      delayComplete = false;
      digitalWrite(leds[i],LOW);
      if(i>=1){
        i--;
      }
    }
  }
