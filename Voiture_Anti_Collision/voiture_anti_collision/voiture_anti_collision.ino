/***********************************************************************************************/
/*                        VOITURE AUTOMATISÃ‰E ANTI-COLLISION v1.0                              */
/*                                                                                             */
/*                La voiture explore en ligne droite et Ã©vite les obstacles.                   */ 
/*                                                                                             */
/*                <!> Modifier le fichier "configuration.h" si nÃ©cessaire.<!>                  */
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
#include "AffectationDesEntreesSorties.h"
#include "DeclarationDesVariables.h"
#include "Configuration.h"
#include "DriverLEDs.h"
////#include "DriverMoteur.h"
#include "DriverMoteur-origine.h"
#include "DriverServo.h"
#include "DriverUltraSonicSensor.h"
#include "Initialisation.h"
#include "AlgorithmeQuiChercheUneIssue.h"

/*****************************************************************/
/*                        INITIALISATION                         */
/*****************************************************************/
void setup() {Initialisation();}
//const int potar = 0;  //la broche pour rÃ©gler la vitesse

 //on dÃ©marre moteur en avant et en roue libre
 //   analogWrite(enable, 0);
    
/*****************************************************************/
/*                                                               */
/*                        BOUCLE INFINIE                         */
/*                Algorithme dÃ©finit dans la vidÃ©o.              */
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

