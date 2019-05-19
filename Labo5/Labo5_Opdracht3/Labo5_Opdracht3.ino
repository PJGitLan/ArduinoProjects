const int digit = 5;
const int anodes[] = {6,7,8,9,10,11,12}; // {A,B,C,D,E,F,G}
const int num_array[10][7] = {  { 1,1,1,1,1,1,0 },    // 0
                          { 0,1,1,0,0,0,0 },    // 1
                          { 1,1,0,1,1,0,1 },    // 2
                          { 1,1,1,1,0,0,1 },    // 3
                          { 0,1,1,0,0,1,1 },    // 4
                          { 1,0,1,1,0,1,1 },    // 5
                          { 1,0,1,1,1,1,1 },    // 6
                          { 1,1,1,0,0,0,0 },    // 7
                          { 1,1,1,1,1,1,1 },    // 8
                          { 1,1,1,0,0,1,1 }};   // 9
int i = 0;
int number = 0;

void printTo7Seg(int number){
    for(i=0;i<7;i++){
      digitalWrite(anodes[i],num_array[number][i]);
    }
 }
  
void setup() {
  // put your setup code here, to run once: 
  Serial.begin(9600);
  Serial.setTimeout(10);

  for(i=0; i<7; i++){
    pinMode(anodes[i], OUTPUT);
  }

  pinMode(digit, OUTPUT);
  digitalWrite(digit, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available() > 1){
    number = Serial.parseInt();
    if(number<=9&&number>=0){
      Serial.print("number =");
      Serial.println(number);
      printTo7Seg(number);
    }
  }
}
