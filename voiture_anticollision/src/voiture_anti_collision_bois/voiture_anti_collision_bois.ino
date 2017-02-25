/*
Robot prototype anti collision monté sur 2 CDROM et pate à fixe
le prototype pate à fixe
une vidéo du robot en ordre de marche :


*/
#include <Arduino.h>

#define vitesse2 200
#define vitesse1 200
#define A 500
#define B 300
#define trigPin 2            //Trig Ultrasons (sortie)
#define echoPin 4
int motor_pin1 = 7;//in2
int motor_pin2 = 5;//in1
int motor_pin3 = 10;//in3
int motor_pin4 = 8;//in4
const int Motor2EN =9;//enb
const int Motor1EN =6;//ena
void setup () {
 pinMode(trigPin, OUTPUT);   //Trig est une sortie
 pinMode(echoPin, INPUT);    //Echo est une entrÃ©e
  pinMode(motor_pin1, OUTPUT);
  pinMode(motor_pin2, OUTPUT);
  pinMode(motor_pin3, OUTPUT);
  pinMode(motor_pin4, OUTPUT);
  pinMode(Motor2EN, OUTPUT);
  pinMode(Motor1EN, OUTPUT);
  delay(700);
}

void loop() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);               //Trig dÃ©clenchÃ© 10ms sur HIGH
  digitalWrite(trigPin, LOW);
  // calcul de la distance :
  duration = pulseIn(echoPin, HIGH);   // Distance proportionnelle Ã  la durÃ©e de sortie
  distance = duration * 340 / (2 * 10000); //Vitesse du son thÃ©orique
 if (distance >= 20) {                //Si il n'y a pas d'obstacle Ã  plus de 20cm
    digitalWrite(motor_pin1, LOW);     //Alors il avance
    digitalWrite(motor_pin2, HIGH);
    digitalWrite(motor_pin3, HIGH);
    digitalWrite(motor_pin4, LOW);
analogWrite(Motor2EN, vitesse2);
analogWrite(Motor1EN, vitesse2);
 }
  else {                               //Sinon (si il voit un objet a moin de 20 cm)
    digitalWrite(motor_pin1, HIGH);    //Le robot part en arriere puis 500 milliseconde
    digitalWrite(motor_pin2, LOW);     //aprÃ©s il part vers la gauche.
    digitalWrite(motor_pin3, LOW);
    digitalWrite(motor_pin4, HIGH);
 analogWrite(Motor2EN, vitesse1);
 analogWrite(Motor1EN, vitesse1);
 delay(A*3);
    digitalWrite(motor_pin1, LOW);
    digitalWrite(motor_pin2, HIGH);
    digitalWrite(motor_pin3, LOW);
    digitalWrite(motor_pin4, HIGH);
 analogWrite(Motor2EN, vitesse1);
 analogWrite(Motor1EN, vitesse1);
 delay(B*2);
 }
}
