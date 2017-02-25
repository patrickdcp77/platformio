// ce programme allulme les leds une par une
int pin[6]={1,2,3,4,5,6};
int temp0=100; //tempo pour allumer les leds
int temp1=100; // tempo pour eteindre

void setup(){
for (int i = 0 ; i<=5 ; i++){
pinMode(pin[i], OUTPUT);
}
}

void loop(){
for (int i = 0 ; i<=5 ; i++){ // boucle for pour allumer les leds une par une
digitalWrite(pin[i], HIGH);
delay(temp0);
}
for (int i = 5 ; i>=0 ; i--){
digitalWrite(pin[i], LOW);
delay(temp1);
}
}
