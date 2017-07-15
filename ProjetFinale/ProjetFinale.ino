
#include <Servo.h> 
#include <Ethernet.h>
#define PIR 12

int etat = 0;
int calibrationTime = 10;

/*Ultrason + LED RGB*/
const byte TRIGGER_PIN = 2; // Broche TRIGGER
const byte ECHO_PIN = 3;    // Broche ECHO
int redPin = 11; //PIN LED Rouge
int greenPin = 10; //PIN LED Verte
int bluePin = 9; // PIN LED Bleue

/* Constantes pour le timeout */
const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s


const float SOUND_SPEED = 340.0 / 1000; // Vitesse du son dans l'air en mm/us 
 
Servo myservo;  // Créer un objet servo pour contrôler               
int pos = 0;    // Variable pour stocker la position du servo


  //Ethernet
  //**************
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//IPAddress server(192,168,1,16);  // numeric IP for Google (no DNS)
char server[] = "http://poubelle-connecte.pe.hu/PoubelleAPI/API/v1/poubelles";    // nom adresse api

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);

String data = "{\"sujet\":\"TEST\"}";

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
//End Ethernet
//**************
 
void setup() 
{ 

  Serial.begin(115200);   // Initialise le port série
   
  /* Initialise les broches */
  pinMode(TRIGGER_PIN, OUTPUT);
  digitalWrite(TRIGGER_PIN, LOW); // La broche TRIGGER doit être à LOW au repos
  pinMode(ECHO_PIN, INPUT); //PIN en ENTREE
  pinMode(redPin, OUTPUT); //PIN Led en SORTIE
  pinMode(greenPin, OUTPUT); //PIN Led en SORTIE
  pinMode(bluePin, OUTPUT); //PIN Led en SORTIE
  
   for(int i = 0; i < calibrationTime; i++){
    Serial.print(".");
    delay(400);
  }
  Serial.println();
  Serial.println("Capteur Calibre");
  
  Serial.begin(9600); //Moniteur série
  pinMode(PIR, INPUT); // Broche 12 configurée en entrée
  myservo.attach(9);  


  //Ethernet
  //**************
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 8000)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println("POST /api/addsensor HTTP/1.1");                    
    //client.println("Host: 192.168.1.16");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Connection: close");
    client.println("User-Agent: Arduino/1.0");
    client.print("Content-Length: ");
    client.println("Authorization: 226f791098549052f704eb37b2ae7999");
    client.println(data.length());
    client.println();
    client.print(data);
    client.println(); 
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
    //End Ethernet
  //**************
} 
 
 
void loop() 
{ 

   // Lance une mesure de distance en envoyant une impulsion HIGH de 10µs sur la broche TRIGGER 
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
  
  long measure = pulseIn(ECHO_PIN, HIGH, MEASURE_TIMEOUT); // Mesure le temps entre l'envoi de l'impulsion ultrasonique et son écho (si il existe)
   
 
  float distance_mm = measure / 2.0 * SOUND_SPEED;  // Calcul la distance à partir du temps mesuré 
   
  /* Affiche les résultats en mm, cm et m */
  Serial.print(F("Distance: "));
  Serial.print(distance_mm);
  Serial.print(F("mm ("));
  Serial.print(distance_mm / 10.0, 2);
  Serial.println(F("cm) "));

  if (distance_mm >= 174){
    Serial.println("Poubelle remplie a 0%");
      setColor(0, 255, 0);  // LED Verte
  }

  if (distance_mm < 173 && distance_mm > 131 ){
    Serial.println("Poubelle remplie a 25%");
    setColor(255, 255, 0);  // LED Jaune
    
  }

  if (distance_mm < 130 && distance_mm > 88){
    Serial.println("Poubelle remplie a 50%");
    setColor(255, 255, 0);  // LED Jaune
  }

  if (distance_mm < 120 && distance_mm > 90) {
    Serial.println("Poubelle remplie a 75%");
    setColor(255, 0, 0);  // LED Rouge
  }
  if (distance_mm < 85) {
    Serial.println("Poubelle remplie a 100%");
    setColor(255, 0, 0);  // LED Rouge
  } 
  delay(1000);  //Délai d'attente pour éviter d'afficher trop de résultats à la seconde

  etat = digitalRead(PIR); // Lit l'état de la broche 12
  Serial.print(etat); //Affiche l'état dans le moniteur série
  delay(300); // Délai pour ne pas écrire trop vite dans le moniteur

  if (etat == 1) {
  for(pos = 0; pos < 180; pos += 1)  // Servomoteur de 0 à 180 degrés 
  {                                  // 
    myservo.write(pos);              //  
                             
  } 
    
  }
  if (etat == 0) {
    for(pos = 180; pos>=1; pos-=1)     // Servomoteur de 180 à 0 degrés
  {                                
          myservo.write(pos);   // Dit au servo d'aller à la position "pos" 
                           
  } 
  }

/********
 * Ethernet
 */
    // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    while (true);
  }
   /********
 * End Ethernet
 */
} 

/* Initialisation de la LED */
void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}
