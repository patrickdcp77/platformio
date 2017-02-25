/*
 *  This sketch trys to Connect to the best AP based on a given list
 *
 */


/*
 * PIR sensor tester
 */
//mouv**************** 
int ledPin = 12;                // choose the pin for the LED
int inputPin = 13;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
//mouv******************* 







#include <ESP8266WiFi.h>//////////////////
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2

ESP8266WiFiMulti wifiMulti;
ESP8266WebServer server(80);

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

const int led = 13;

//buffer pour stocker la temperature
char tempString[4];

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(led, 0);
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void handleTemp(){
  digitalWrite(led, 1);
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures();
  Serial.println("DONE");
  Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.println(sensors.getTempCByIndex(0));  
  dtostrf(sensors.getTempCByIndex(0),4,1,tempString);
  server.send(200, "text/plain", tempString);
  digitalWrite(led, 0);
}

void setup() {
     
    Serial.begin(115200);
    delay(10);


//mouv**********************
pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
//mouv******************








  
    wifiMulti.addAP("MLC", "ea1a5ead7cd63");
    wifiMulti.addAP("maison", "b5c84f1cec");
    //wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3");

    Serial.println("Connecting Wifi...");
    while(wifiMulti.run() != WL_CONNECTED) {
      Serial.print('.');
      delay(100);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.on("/temp", handleTemp);
    server.onNotFound(handleNotFound);
    
    server.begin();
    sensors.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();



//mouv************************
val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  } else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
   
  }
 //mouv***********************






    
}
