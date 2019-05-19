import processing.serial.*;

Serial myPort;  
int val = 0;      

void setup() {
  size(800 , 600); //Maakt een window van 800*600 px
  background(200); //Kleurt de achtergrond grijs
  ellipseMode(CENTER); //Past de locatie aan waaruit de ellips wordt getekend. 
  fill(200);//Past de achtergrond van de cirkel aan.
  ellipse( 400, 300, 200, 200);//maakt een elips met ellipse(positie horizontaal, positie verticaal, breedte, hoogte)
  textSize(12); //grootte van de text
  fill(0); //defineert kleur van opvulling van een object (in dit geval text)
  textAlign(CENTER);//wordt gebruikt om text te aligneren. De eerste parameter wordt gebruikt om horizontaal te aligneren en de tweede om verticaal te aligneren.
  text("push a button", 400, 300); //Schrijft een array op het scherm op positie x=400 en y=300
  

  val = Serial.list().length; //Checkt of er seriële poorten zijn aangesloten 

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
    print(i + ": "); //Print de plaats van de serële poort in de array serial.list(); 
    println(Serial.list()[i]); //Print welke poort er wordt gebruikt wordt.
  }
  myPort = new Serial(this, Serial.list()[0], 9600);
  println("Seriële poort geselecteerd: " + Serial.list()[0]); //Selecteert de eerst beschikbare poort.

} 

void draw () { 
  if ( myPort.available() > 0) {        
    val = myPort.read();                 
  }
  
  if( val == '1' ){
    float x = random(800); //genereert een float tussen 0 en 800
    float y = random(600); //genereert een float tussen 0 en 600
    float r = random(50)+10; //genereert een float tussen 10 en 60 
    fill( random(255), random(255), random(255)); //genereert een random kleur voor het elips
    ellipse( x, y, r, r);
    val = 0;
  }
  
  if( val == '2' ){
    background(200); //overwrite wat er al op het scherm geschreven staat
    val = 0;
  }
}
