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
