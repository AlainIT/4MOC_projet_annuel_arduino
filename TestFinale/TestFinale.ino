
#include <Servo.h> 
#define PIR 12

int etat = 0;
int calibrationTime = 10;
 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
 
int pos = 0;    // variable to store the servo position 
 
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
  for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
                             
  } 
    
  }
  if (etat == 0) {
    for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
          myservo.write(pos);   // tell servo to go to position in variable 'pos' 
                           
  }
     
  }
   
} 
