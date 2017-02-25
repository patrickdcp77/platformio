#include <ESP8266WiFi.h>
#include <Base64.h>
#include <OneWire.h>
#include <DallasTemperature.h>


void setup() {
  // put your setup code here, to run once:

}
void loop() {
  float temp;
 delay(500);
 //on lance la lecture de la temperature
 do{
 // DS18B20.requestTemperatures();
  temp=DS18B20.getTempCByIndex(0);
 } while (temp == 85.0 || temp== (127.0));
 Serial.print("temperature: ");
 Serial.println(temp);

 // on se connecte à domoticz

while (!esp_01.connect(ip,8080)) {
  Serial.print(".");
}

Serial.println("connexion domotics réalisée");

// transmission des paramètre à domoticz
esp_01.println("http://192.168.0.42:8080/json.htm?type=command&param=udevice&idx=2&nvalue=0&svalue=" + String(temp));
  
Serial.println("/json.htm?type=command&param=udevice&idx=2&nvalue=0&svalue=" + String(temp));
}
