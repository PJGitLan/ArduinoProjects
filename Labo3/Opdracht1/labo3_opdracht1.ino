const int V_in = A3;
int i = 0;

void setup() {
  // put your setup code here, to run once:  
  pinMode(V_in, INPUT);
  Serial.begin(9600);
   
}

void loop() {
  // put your main code here, to run repeatedly:
  float voltage = analogRead(V_in)*(5.0/1023.0);
  float aantalPlus = voltage/0.1;
  
  Serial.print((String)voltage+": 0");
  for(i=0;i<=aantalPlus-1;i++){
    Serial.print("+");
  }
  Serial.println("");
  
  
}
