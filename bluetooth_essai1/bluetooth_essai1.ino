//********************************************************
//  Module de communication BlueTooth HC-06 avec Arduino
//    Rolland 11 01 2014
//    tiptopboards.com
// Programme de démo, le module HT-06 envoie des nouvelles données 
// chaque seconde au PC
//
//********************************************************
 int counter = 0;
 unsigned long time;
 
void setup() {
    Serial.begin(9600); //Module bluetooth par défaut 9600, N, 8, 1
    pinMode(13, OUTPUT);   //Led 13 de témoin OK
}
 
void loop() {
    digitalWrite(13, HIGH);   // LED 13 ON
    delay(100);             
    digitalWrite(13, LOW);    // Off
     
    Serial.print("Test module HC-06 bluetooth ");
    Serial.print(++counter);
    Serial.print(" " );
    time = millis();
    Serial.println(time);
    delay(1000);  //Un envoi de données par seconde
}

