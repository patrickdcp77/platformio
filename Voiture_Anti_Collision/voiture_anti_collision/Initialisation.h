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
    
    pinMode(DOUT_LED_ROUGE, OUTPUT);
    pinMode(DOUT_LED_VERTE, OUTPUT); 
    
    SERVO.attach(PWM_SERVO);
    
    RegardeToutDroit();
  }
