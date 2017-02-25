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
