/***********************************************************************************************/
/*                        VOITURE AUTOMATISÉE ANTI-COLLISION v1.0                              */
/*                                                                                             */
/*                La voiture explore en ligne droite et évite les obstacles.                   */ 
/*                                                                                             */
/*                <!> Modifier le fichier "configuration.h" si nécessaire.<!>                  */
/*                                                                                             */
/*                                     Rev SW: v1.0                                            */
/*                              Version HW compatible: v1.0                                    */
/*                                                                                             */
/*                           le 27/02/2015 par Alexandre Pailhoux                              */
/*                                                                                             */
/* http://les-electroniciens.com/videos/realisation-voiture-automatisee-anti-collision-arduino */
/***********************************************************************************************/

/*****************************************************************/
/*                         LIBRAIRIES                            */
/*****************************************************************/
#include <Servo.h> 
#include "Arduino.h"

/*
#include "AffectationDesEntreesSorties.h"
#include "DeclarationDesVariables.h"
#include "Configuration.h"
#include "DriverLEDs.h"
#include "DriverMoteur.h"
#include "DriverServo.h"
#include "DriverUltraSonicSensor.h"
#include "Initialisation.h"
#include "AlgorithmeQuiChercheUneIssue.h"

*/
/***************************************************/
/*              Déclarations des E/S               */
/*      Schéma dans la description de la vidéo     */
/***************************************************/
const char DOUT_MOTEUR_GAUCHE_EN  = 6;
const char D0UT_MOTEUR_GAUCHE_1   = 5;
const char D0UT_MOTEUR_GAUCHE_2   = 7;

const char DOUT_MOTEUR_DROIT_EN   = 3;
const char D0UT_MOTEUR_DROIT_3    = 10;
const char D0UT_MOTEUR_DROIT_4    = 8;

const char DOUT_ULTRASONIC_TRIG   = 2;
const char DIN_ULTRASONIC_ECHO    = 4;

const char PWM_SERVO              = 9;

const char DOUT_LED_ROUGE           = 11;
const char DOUT_LED_ORANGE          = 12;
const char DOUT_LED_VERTE           = 13;

SERVOu.attach(PWM_SERVO);
/**************************************************/
/*   Variables à configurer selon votre voiture   */
/**************************************************/
const int potar = 0;  //la broche pour régler la vitesse


int completementAGauche = 135; // Position du Servo considérée comme complètement à gauche (pour chercher une issue).
int aGauche             = 120; // Position du Servo considérée comme à gauche: 120°.

int toutDroit           = 90;  // Position du Servo considérée comme en face: 90° <!> 60° si votre Servo n'est pas inversé mécaniquement.

int aDroite             = 60;  // Position du Servo considérée comme à gauche: 60° <!> 90° si votre Servo n'est pas inversé mécaniquement
int completementADroite = 45;  // Position du Servo considérée comme complètement à droite (pour chercher une issue).

int dureePostionnementServo = 250;// Durée d'attente en milliseconde pour considérer que le Servo à atteint sa position

int seuilDistance       = 30;    // Si un objet est à moins de 30cm alors c'est considéré comme un obstacle
int dureePourPivoter    = 200;   // Durée d'une rotation à gauche ou à droite en milliseconde
int dureeDunDemiTour    = 600;   // Durée d'un demi-tour en milliseconde

/**********************************/
/*   Déclarations des variables   */
/**********************************/
int     distanceMesuree         = 0;      // Variable qui stockera la distance mesurée
boolean laVoitureEstAlArret     = true;   // Variable qui retient si la voiture est à l'arrêt
int vitesse = 150;  //analogRead(potar);

Servo SERVO; 

/*****************************************************************/
/*                        INITIALISATION                         */
/*****************************************************************/
void setup() {Initialisation();}
//const int potar = 0;  //la broche pour régler la vitesse

 //on démarre moteur en avant et en roue libre
 //   analogWrite(enable, 0);
    
/*****************************************************************/
/*                                                               */
/*                        BOUCLE INFINIE                         */
/*                Algorithme définit dans la vidéo.              */
/*                                                               */
/*****************************************************************/
void loop() {   
  


   
   RegardeToutDroit(); 
   if(LaVoieEstLibre())
   {
     RegardeAGauche();      
     if(LaVoieEstLibre()) 
     { 
       RegardeToutDroit();       
       if(LaVoieEstLibre()) 
       {
         RegardeADroite();          
         if(LaVoieEstLibre()) 
         {
           if(laVoitureEstAlArret)
           {AvanceEnLigneDroite();}
         }
         else{ChercheUneIssue();}
       }
       else{ChercheUneIssue();}
     }     
     else{ChercheUneIssue();} 
   }  
   else{ChercheUneIssue();} 
   delay(1);

  
   
}

/************************************************/
/*        Fonction qui cherche une issue        */
/*       Algorithme définit dans la vidéo.      */
/************************************************/
void ChercheUneIssue()
{
 Arret(); 
 ChercheUneIssueAGauche();
 if(LaVoieEstLibre()) 
 {                    
   PivoteAGauche(); 
 }
 else
 {
   ChercheUneIssueADroite();
   if(LaVoieEstLibre())
   {
      PivoteADroite();
   }
   else
   {
      DemiTour();
   }
 }
}

/******************************************************/
/*           Initialisation de la voiture             */
/******************************************************/
  
  void Initialisation()
  {
    pinMode(D0UT_MOTEUR_GAUCHE_1, OUTPUT);
    digitalWrite(D0UT_MOTEUR_GAUCHE_1,LOW);
    pinMode(D0UT_MOTEUR_GAUCHE_2, OUTPUT);
    digitalWrite(D0UT_MOTEUR_GAUCHE_2,LOW);
    
    pinMode(D0UT_MOTEUR_DROIT_3 , OUTPUT);
    digitalWrite(D0UT_MOTEUR_DROIT_3,LOW);
    pinMode(D0UT_MOTEUR_DROIT_4 , OUTPUT);
    digitalWrite(D0UT_MOTEUR_DROIT_4,LOW);
    
    pinMode(DOUT_ULTRASONIC_TRIG, OUTPUT);
    digitalWrite(DOUT_ULTRASONIC_TRIG,LOW);
    pinMode(DIN_ULTRASONIC_ECHO, INPUT);
    
    digitalWrite(DOUT_MOTEUR_GAUCHE_EN,LOW);
    digitalWrite(DOUT_MOTEUR_DROIT_EN,LOW);

// analogWrite(DOUT_MOTEUR_GAUCHE_EN,0);
  //analogWrite(DOUT_MOTEUR_DROIT_EN,0);
    
    
    pinMode(DOUT_LED_ROUGE, OUTPUT);
    pinMode(DOUT_LED_VERTE, OUTPUT); 
    
    
    
    RegardeToutDroit();
  }




/*************************************************************/
/*   Fonction qui fait avancer la voiture en ligne droite    */
/*************************************************************/
void AvanceEnLigneDroite()
{
  digitalWrite(D0UT_MOTEUR_GAUCHE_1,HIGH);
  digitalWrite(D0UT_MOTEUR_GAUCHE_2,LOW);  
  digitalWrite(D0UT_MOTEUR_DROIT_3,HIGH);
  digitalWrite(D0UT_MOTEUR_DROIT_4,LOW);
  
  digitalWrite(DOUT_MOTEUR_GAUCHE_EN,HIGH);
  digitalWrite(DOUT_MOTEUR_DROIT_EN,HIGH);


 // analogWrite(DOUT_MOTEUR_GAUCHE_EN,vitesse);
  //analogWrite(DOUT_MOTEUR_DROIT_EN,vitesse);
  
  laVoitureEstAlArret = false;   

  allumeLEDVerte();  
}

/*************************************************/
/*       Fonction qui stoppe la voiture          */
/*************************************************/
void Arret()
{
  digitalWrite(D0UT_MOTEUR_GAUCHE_1,LOW);
  digitalWrite(D0UT_MOTEUR_GAUCHE_2,LOW);  
  digitalWrite(D0UT_MOTEUR_DROIT_3,LOW);
  digitalWrite(D0UT_MOTEUR_DROIT_4,LOW);
  
  digitalWrite(DOUT_MOTEUR_GAUCHE_EN,LOW);
  digitalWrite(DOUT_MOTEUR_DROIT_EN,LOW);

  //analogWrite(DOUT_MOTEUR_GAUCHE_EN,0);
 // analogWrite(DOUT_MOTEUR_DROIT_EN,0);
  
  laVoitureEstAlArret = true;  
 
  allumeLEDOrange(); 
}

/******************************************************/
/*   Fonction qui fait pivoter la voiture à gauche    */
/******************************************************/
void PivoteAGauche()
{
  digitalWrite(D0UT_MOTEUR_GAUCHE_1,LOW);
  digitalWrite(D0UT_MOTEUR_GAUCHE_2,HIGH);  
  digitalWrite(D0UT_MOTEUR_DROIT_3,HIGH);
  digitalWrite(D0UT_MOTEUR_DROIT_4,LOW);
  
  digitalWrite(DOUT_MOTEUR_GAUCHE_EN,HIGH);
  digitalWrite(DOUT_MOTEUR_DROIT_EN,HIGH);


  //analogWrite(DOUT_MOTEUR_GAUCHE_EN,vitesse);
  //analogWrite(DOUT_MOTEUR_DROIT_EN,vitesse);
  
  delay(dureePourPivoter);
  
  digitalWrite(DOUT_MOTEUR_GAUCHE_EN,LOW);
  digitalWrite(DOUT_MOTEUR_DROIT_EN,LOW);
  
  //analogWrite(DOUT_MOTEUR_GAUCHE_EN,0);
  //analogWrite(DOUT_MOTEUR_DROIT_EN,0);
  
  
  digitalWrite(D0UT_MOTEUR_GAUCHE_1,LOW);
  digitalWrite(D0UT_MOTEUR_GAUCHE_2,LOW);  
  digitalWrite(D0UT_MOTEUR_DROIT_3,LOW);
  digitalWrite(D0UT_MOTEUR_DROIT_4,LOW);
  
  laVoitureEstAlArret = true;  
  
  allumeLEDOrange();
}

/******************************************************/
/*   Fonction qui fait pivoter la voiture à droite    */
/******************************************************/
void PivoteADroite()
{
  digitalWrite(D0UT_MOTEUR_GAUCHE_1,HIGH);
  digitalWrite(D0UT_MOTEUR_GAUCHE_2,LOW);  
  digitalWrite(D0UT_MOTEUR_DROIT_3,LOW);
  digitalWrite(D0UT_MOTEUR_DROIT_4,HIGH); 
  
  delay(dureePourPivoter);
  
  digitalWrite(DOUT_MOTEUR_GAUCHE_EN,LOW);
  digitalWrite(DOUT_MOTEUR_DROIT_EN,LOW);


  //analogWrite(DOUT_MOTEUR_GAUCHE_EN,0);
  //analogWrite(DOUT_MOTEUR_DROIT_EN,0);
  
  digitalWrite(D0UT_MOTEUR_GAUCHE_1,LOW);
  digitalWrite(D0UT_MOTEUR_GAUCHE_2,LOW);  
  digitalWrite(D0UT_MOTEUR_DROIT_3,LOW);
  digitalWrite(D0UT_MOTEUR_DROIT_4,LOW);
  
  laVoitureEstAlArret = true;  
  
  allumeLEDOrange();
}

/******************************************************/
/*      Fonction qui fait faire un demi-tour          */
/******************************************************/
void DemiTour()
{
  digitalWrite(D0UT_MOTEUR_GAUCHE_1,HIGH);
  digitalWrite(D0UT_MOTEUR_GAUCHE_2,LOW);  
  digitalWrite(D0UT_MOTEUR_DROIT_3,LOW);
  digitalWrite(D0UT_MOTEUR_DROIT_4,HIGH);
  digitalWrite(DOUT_MOTEUR_GAUCHE_EN,HIGH);
  digitalWrite(DOUT_MOTEUR_DROIT_EN,HIGH);


  //analogWrite(DOUT_MOTEUR_GAUCHE_EN,vitesse);
  //analogWrite(DOUT_MOTEUR_DROIT_EN,vitesse);
  
  
  delay(dureeDunDemiTour);
  
  digitalWrite(DOUT_MOTEUR_GAUCHE_EN,LOW);
  digitalWrite(DOUT_MOTEUR_DROIT_EN,LOW);


  //analogWrite(DOUT_MOTEUR_GAUCHE_EN,0);
  //analogWrite(DOUT_MOTEUR_DROIT_EN,0);
  
  digitalWrite(D0UT_MOTEUR_GAUCHE_1,LOW);
  digitalWrite(D0UT_MOTEUR_GAUCHE_2,LOW);  
  digitalWrite(D0UT_MOTEUR_DROIT_3,LOW);
  digitalWrite(D0UT_MOTEUR_DROIT_4,LOW);
  
  laVoitureEstAlArret = true;  
  
  allumeLEDRouge();
}





/*************************************************************/
/*             Fonction qui allume la LED Verte              */
/*************************************************************/
void allumeLEDVerte()
{
  digitalWrite(DOUT_LED_VERTE,HIGH);
  digitalWrite(DOUT_LED_ORANGE,LOW);
  digitalWrite(DOUT_LED_ROUGE,LOW);  
}
/*************************************************************/
/*             Fonction qui allume la LED Orange              */
/*************************************************************/
void allumeLEDOrange()
{
  digitalWrite(DOUT_LED_VERTE,LOW);
  digitalWrite(DOUT_LED_ORANGE,HIGH);
  digitalWrite(DOUT_LED_ROUGE,LOW); 
}
/*************************************************************/
/*             Fonction qui allume la LED Rouge              */
/*************************************************************/
void allumeLEDRouge()
{
  digitalWrite(DOUT_LED_VERTE,LOW);
  digitalWrite(DOUT_LED_ORANGE,LOW);
  digitalWrite(DOUT_LED_ROUGE,HIGH); 
}
/*************************************************************/
/*             Fonction qui éteint les LEDS              */
/*************************************************************/
void eteintLEDs()
{
  digitalWrite(DOUT_LED_VERTE,LOW);
  digitalWrite(DOUT_LED_ORANGE,LOW);
  digitalWrite(DOUT_LED_ROUGE,LOW); 
}

/******************************************************/
/*              Déclaration du Module                 */
/******************************************************/


/******************************************************/
/*  Fonction qui ordonne au Servo de regarder en face */
/******************************************************/
void RegardeToutDroit()
{
  SERVO.write(toutDroit); 
  delay(dureePostionnementServo);
}

/*******************************************************/
/*  Fonction qui ordonne au Servo de regarder à gauche */
/*******************************************************/
void RegardeAGauche()
{
  SERVO.write(aGauche); 
  delay(dureePostionnementServo);
}

/*******************************************************/
/*  Fonction qui ordonne au Servo de regarder à droite */
/*******************************************************/
void RegardeADroite()
{
  SERVO.write(aDroite); 
  delay(dureePostionnementServo);
}

/*******************************************************/
/*  Fonction qui ordonne au Servo de regarder à gauche */
/*******************************************************/
void ChercheUneIssueAGauche()
{
  SERVO.write(completementAGauche); 
  delay(dureePostionnementServo);
}

/*******************************************************/
/*  Fonction qui ordonne au Servo de regarder à droite */
/*******************************************************/
void ChercheUneIssueADroite()
{
  SERVO.write(completementADroite); 
  delay(dureePostionnementServo);
}

/**********************************************/
/*  Fonction qui détecte si la voie est libre */
/*                                            */
/*  Retourne True si la voie est libre        */
/*  Retourne False s'il y a un obstacle       */
/**********************************************/
boolean LaVoieEstLibre()
{
  digitalWrite(DOUT_ULTRASONIC_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(DOUT_ULTRASONIC_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(DOUT_ULTRASONIC_TRIG, LOW);
  
  distanceMesuree = pulseIn(DIN_ULTRASONIC_ECHO, HIGH) / 58;  
  
  /* Pour test...
  if (distanceMesuree < 2 || 500 < distanceMesuree)
  {
    distanceMesuree = seuilDistance+1;
  }*/
  
  if(seuilDistance < distanceMesuree)  
  {return true;}
  else
  {return false;}   
}



