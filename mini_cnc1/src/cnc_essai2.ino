/*
  Mini Traceur Arduino (CNC Plotter)
  Test de l'axe X
  Projets DIY 02/2016
*/
#include <Arduino.h>
#include <Stepper.h>			//Librairie pour piloter les moteurs pas à pas

const int pasParRotation = 20; 	//Nombre de pas pour réaliser un tour

//Pins sur lesquels est relié le moteur de l'axe X
Stepper myStepperX(pasParRotation, 8,9,10,11);

void setup() {
 myStepperX.setSpeed(100);		//Vitesse de déplacement
 //Indiquer le nombre de pas que le moteur pas à pas doit faire
 //Généralement un moteur de lecteur CD/DVD peut faire 250 pas au maxium
 //Indiquer un chiffre négatif pour inverser le sens de déplacement
 myStepperX.step(100);
 delay(100);
}

void loop() {}
