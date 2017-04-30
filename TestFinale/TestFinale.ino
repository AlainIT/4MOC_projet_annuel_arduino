
#include <Servo.h> 
#define PIR 12

int etat = 0;
int calibrationTime = 10;
 
Servo myservo;  // Créer un objet servo pour contrôler 
                
 
int pos = 0;    // Variable pour stocker la position du servo
 
void setup() 
{ 
   for(int i = 0; i < calibrationTime; i++){
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("Capteur Calibre");
  
  Serial.begin(9600); //Moniteur série
  pinMode(PIR, INPUT); // Broche 12 configurée en entrée
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
} 
 
 
void loop() 
{ 

  etat = digitalRead(PIR); // Lit l'état de la broche 12
  Serial.print(etat); //Affiche l'état dans le moniteur série
  delay(1500); // Délai pour ne pas écrire trop vite dans le moniteur

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
   
} 
