/******************************************************/
/*              Déclaration du Module                 */
/******************************************************/
Servo SERVO; 

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
