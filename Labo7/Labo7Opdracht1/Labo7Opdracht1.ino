byte byteVar = 115;
int intVar = 400;
char charVar = 'p';
float floatVar = 0.3445;

byte *b;
int *i;
char *c;
float *f;



void setup() {
  // put your setup code here, to run once:
  b = &byteVar;
  i = &intVar;
  c = &charVar;
  f = &floatVar;
  
  Serial.begin(9600);
  Serial.println("type  adres inhoud");
  Serial.println("-------------------------");
  Serial.print("byte");
  Serial.print("  ");
  Serial.print(int(b),HEX);
  Serial.print("   ");
  Serial.print(*b);
  Serial.println();
  
  Serial.print("int ");
  Serial.print("  ");
  Serial.print(int(i),HEX);
  Serial.print("   ");
  Serial.print(*i);
  Serial.println();

  Serial.print("char");
  Serial.print("  ");
  Serial.print(int(c),HEX);
  Serial.print("   ");
  Serial.print(*c);
  Serial.println();

  Serial.print("float");
  Serial.print(" ");
  Serial.print(int(f),HEX);
  Serial.print("   ");
  Serial.print(*f);
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:

}
