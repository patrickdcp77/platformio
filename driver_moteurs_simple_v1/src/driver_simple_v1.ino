/*
commande d'un moteur
potentiometre: aiguille à A0 les autres au + et au -
standby OUTPUT et HIGH
*/

#include <Arduino.h>
const int enable = 9; //la PWM
const int in1 = 8;//2;    //les broches de signal
const int in2 = 5;//4;
const int potar = 0;  //la broche pour régler la vitesse
int STBY = 7; //standby  le standby sur le module

void setup()
{
   pinMode(enable, OUTPUT);
   pinMode(in1, OUTPUT);
   pinMode(in2, OUTPUT);
   Serial.begin(115200);


   pinMode(STBY, OUTPUT);
   digitalWrite(STBY, HIGH); //disable standby

   //on démarre moteur en avant et en roue libre
   analogWrite(enable, 0);
   digitalWrite(in1, LOW);
   digitalWrite(in2, HIGH);
}

void loop()
{

   int vitesse = analogRead(potar);

   //dans le sens positif
   if(vitesse > 512)
   {
       //on décale l'origine de 512
       vitesse -= 512;
       //le moteur va dans un sens
       digitalWrite(in1, LOW);
       digitalWrite(in2, HIGH);
       Serial.print("+");
   }
   else //dans l'autre sens
   {
       //de même on décale pour que la vitesse augmente en s'éloignant de 512
       vitesse = 512-vitesse;
       //le moteur va dans l'autre sens
       digitalWrite(in1, HIGH);
       digitalWrite(in2, LOW);
       Serial.print("-");
   }

   //pour rester dans l'intervalle [0;255] (sinon on est dans [0;512])
   vitesse /= 2;
   //envoie la vitesse
   analogWrite(enable, vitesse);

   //et l'affiche
   Serial.println(vitesse);
   delay(50);
}
