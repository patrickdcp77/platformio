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
  
  delay(dureePourPivoter);
  
  digitalWrite(DOUT_MOTEUR_GAUCHE_EN,LOW);
  digitalWrite(DOUT_MOTEUR_DROIT_EN,LOW);
  
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
  
  delay(dureeDunDemiTour);
  
  digitalWrite(DOUT_MOTEUR_GAUCHE_EN,LOW);
  digitalWrite(DOUT_MOTEUR_DROIT_EN,LOW);
  
  digitalWrite(D0UT_MOTEUR_GAUCHE_1,LOW);
  digitalWrite(D0UT_MOTEUR_GAUCHE_2,LOW);  
  digitalWrite(D0UT_MOTEUR_DROIT_3,LOW);
  digitalWrite(D0UT_MOTEUR_DROIT_4,LOW);
  
  laVoitureEstAlArret = true;  
  
  allumeLEDRouge();
}
