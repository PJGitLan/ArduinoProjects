#include <NewPing.h>

const int echoPin = 3;
const int trigPin = 5;
int maxDistance = 200;

int previousDistance = 0;
int currentDistance = 0;

NewPing sonar(trigPin, echoPin, maxDistance); // NewPing setup of pins and maximum distance.

void setup() {
  Serial.begin(9600); // Open serial monitor at 115200 baud to see ping results.
}

void loop() {
  delay(35);                     // Wait 500ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  currentDistance = floor(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
   if(currentDistance != previousDistance){
    previousDistance =  currentDistance;
    Serial.print("Ping: ");
    Serial.print(currentDistance);
    Serial.println("cm");
   }
}
