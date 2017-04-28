int calibrationTime = 30;

int inputPin = 2;               // PIN Capteur PIR
int servoPin = 8;               // PIN du servoMoteur
int pirState = LOW;             // Aucun mouvement detecté
int val = 0;                    // Variable pour lire le statut du PIN

void setup() {
  pinMode(servoPin,OUTPUT);// on prépare le pin en mode OUTPUT
  digitalWrite(servoPin,LOW); // on l'initialise à l'état bas
  pinMode(inputPin, INPUT);     // Déclare le capteur en mode INPUT
  Serial.begin(9600);
  
  Serial.print("Calibration du capteur ");
  for(int i = 0; i < calibrationTime; i++){
    Serial.print(".");
    delay(1000);
  }
}
void loop(){

    for (int position = 0; position <= 180; position++) { // Pour faire bouger le bras de 0° à 180° 
    monServomoteur.write(position);
    delay(15);

    if (pirState == LOW) {
      // Mouvement detecté 
      Serial.println("Ouverture de la poubelle");
      pirState = HIGH;
    }
  }
    
