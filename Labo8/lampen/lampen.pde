import processing.serial.*;

Serial myPort;  
int val = 0;  
boolean overButtonR = false;
boolean overButtonG = false;
boolean overButtonB = false;
boolean overButtonRoff = false;
boolean overButtonGoff = false;
boolean overButtonBoff = false;

void setup() {
  size(190 , 150);
  textAlign(CENTER);
  background(200);
  
  fill(255,0,0);
  rect(10,10,50,50); 
  fill(0,255,0);
  rect(70,10,50,50); 
  fill(0,0,255);
  rect(130,10,50,50); 
  
  fill(0);
  stroke(255, 0, 0);
  rect(10,70,50,50); 
  stroke(0,255,0);
  rect(70,70,50,50); 
  stroke(0,0,255);
  rect(130,70,50,50); 
  
val = Serial.list().length;
  if(val == 0){
    println("Geen seriële poorten gevonden");
    exit();
    return;
  }else if(val > 1){
    println("Meer dan 1 seriële poort gevonden.");
  }
  println("Seriële poorten gedetecteerd: ");
  for(int i=0; i<val; i++)
  {
    print(i + ": ");
    println(Serial.list()[i]);
  }
  myPort = new Serial(this, Serial.list()[0], 9600);
  println("Seriële poort geselecteerd: " + Serial.list()[0]);  

} 

void mousePressed() { //De mousePressed() functie start als een muis knop wordt ingedrukt //De mousePressed variable is een boolean die true geeft wanneer een muisknop is ingedrukt en false wanneer niets is ingedrukt. 
  if (overButtonR) { 
    myPort.write("r=1\n");
  }
  if (overButtonG) { 
    myPort.write("g=1\n");
  }
  if (overButtonB) { 
    myPort.write("b=1\n");    
  }
  if (overButtonRoff) { 
    myPort.write("r=0\n");
  }
  if (overButtonGoff) { 
    myPort.write("g=0\n");
  }
  if (overButtonBoff) { 
    myPort.write("b=0\n");    
  }
  
}

void draw() {

}

void mouseMoved() { //Deze functie start wanneer de muis beweegt en niet ingedrukt is
  checkButtons(); 
}
  
void mouseDragged() {//Deze functie start wanneer de muis beweegt en wel ingedrukt is
  checkButtons(); 
}

void checkButtons() {
  print(mouseX); //mouseX geeft de horizontale posite van de muis in het venster //print() zorgt ervoor dat de waarden in de console geschreven worden
  print(" ");
  println(mouseY);//mouseY geeft de verticale posite van de muis in het venster
 
 //Hieronder wordt gecheckt of de muis zich boven een vlak bevindt als dit zo is worden de juiste booleans/flags veranderd en de text onderaan in het scherm. 
  if (mouseX > 10 && mouseX < 60 && mouseY > 10 && mouseY < 60) {
    overButtonR = true;   
    text("  Rood aan  ",95,140);
  } else {
    overButtonR = false;
  }
  if (mouseX > 70 && mouseX < 120 && mouseY > 10 && mouseY < 60) {
    overButtonG = true;   
    text("  Groen aan  ",95,140);
  } else {
    overButtonG = false;
  }
  if (mouseX > 130 && mouseX < 180 && mouseY > 10 && mouseY < 60) {
    overButtonB = true;   
    text("  Blauw aan  ",95,140);
  } else {
    overButtonB = false;
  }
  
  if (mouseX > 10 && mouseX < 60 && mouseY > 70 && mouseY < 120) {
    overButtonRoff = true;   
    text("  Rood uit  ",95,140);
  } else {
    overButtonRoff = false;
  }
  if (mouseX > 70 && mouseX < 120 && mouseY > 70 && mouseY < 120) {
    overButtonGoff = true;   
    text("  Groen uit  ",95,140);
  } else {
    overButtonGoff = false;
  }
  if (mouseX > 130 && mouseX < 180 && mouseY > 70 && mouseY < 120) {
    overButtonBoff = true;   
    text("  Blauw uit  ",95,140);
  } else {
    overButtonBoff = false;
  }
  
  if(!(overButtonR || overButtonG || overButtonB || overButtonRoff || overButtonGoff || overButtonBoff)){
    fill(200);
    noStroke();
    rect(60,130,70,20);
    stroke(0);
    fill(0);
  } //indien bovenstaande functie in commentaar staat dan wordt het scherm niet gereset en wordt alle text over elkaar geschreven
}
