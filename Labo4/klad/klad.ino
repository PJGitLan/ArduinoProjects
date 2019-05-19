static int digits[] = {2,3,4,5};
static int anodes[] = {6,7,8,9,10,11,12}; // {A,B,C,D,E,F,G}

static int num_array[10][7] = {  { 1,1,1,1,1,1,0 },    // 0
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
int j = 0;
int k = 0;

void setup() {
  // put your setup code here, to run once:
  for(i=0; i<4; i++){
    pinMode(digits[i], OUTPUT);
  }
  
  for(i=0; i<7; i++){
    pinMode(anodes[i], OUTPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  for(j=0;j<4;j++){
   for(i=0;i<7;i++){
      digitalWrite(anodes[i],HIGH);
      delay(1000);
    }
  }
}
