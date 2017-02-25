#include <Arduino.h>
#include <stdlib.h>
#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <OneWire.h>
#include <DallasTemperature.h>
 
//#include "config.h"
 
WiFiClient net;
MQTTClient mqtt;
 
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;
 
OneWire oneWire(tempPin);
DallasTemperature sensors(&oneWire);
 
int state = 0;
unsigned long lastTime;
unsigned long startTime;
 
bool presenceDetectee = false;
bool lastPresence = false;
 
void presenceNonDetecteeInterrupt();
void presenceDetecteeInterrupt();
 
String macToStr(const uint8_t* mac) {
    String result;
    for (int i = 0; i < 6; ++i) {
        result += String(mac[i], 16);
        if (i < 5)
            result += ':';
    }
    return result;
}
 
void wifiConnect() {
    long t0 = millis();
 
    Serial.print("Connecting WiFi, timeout is ");
    Serial.print(connectTimeout);
    Serial.print("ms");
 
    WiFi.begin("maison", "b5c84f1cec");
 
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
        delay(100);
        if ((millis() - t0) > connectTimeout) {
            Serial.println(" Failed. Restarting...");
            delay(1000);
            ESP.restart();
        }
    }
    Serial.println(" Connected !");
    Serial.print("Local IP: ");
    Serial.println(WiFi.localIP());
}
 
void mqttConnect() {
    long t0 = millis();
    uint8_t mac[6];
    String clientName = "esp-";
    Serial.print("Connecting MQTT...");
    WiFi.macAddress(mac);
    clientName += macToStr(mac);
    clientName += "-";
    clientName += String(micros() & 0xff, 16);
 
    mqtt.begin(mqttServer, net);
 
    if (!mqtt.connect((char*) clientName.c_str())) {
        Serial.println(" Failed. Restarting...");
        delay(connectTimeout);
        ESP.restart();
    }
    Serial.println(" Connected !");
}
 
void presenceDetecteeInterrupt(){
    Serial.println("Presence detectee");
    presenceDetectee = true;
    detachInterrupt(presence);
    attachInterrupt(presence, presenceNonDetecteeInterrupt, FALLING);
}
 
void presenceNonDetecteeInterrupt(){
    Serial.println("Presence Non detectee");
    presenceDetectee = false;
    detachInterrupt(presence);
    attachInterrupt(presence, presenceDetecteeInterrupt, RISING);
}
 
float getTemp(){
    Serial.print("Requesting temperatures... ");
    sensors.requestTemperatures();
    Serial.print("Done, temperature is ");
    float t = sensors.getTempCByIndex(0);
    Serial.print(t);
    Serial.println("C");
    return t;
}
 
void initIO() {
    Serial.print("Init I/O... ");
    pinMode(led, OUTPUT);
    pinMode(ADC, INPUT);
    pinMode(presence, INPUT);
    pinMode(tempPin, INPUT_PULLUP);
 
    digitalWrite(led, HIGH);
    delay(1000);
 
    attachInterrupt(presence, presenceDetecteeInterrupt, RISING);
    sensors.begin();
    Serial.println("Done");
}
 
void setup() {
    long next;
    long t0 = millis();
    startTime = millis();
 
    Serial.begin(115200);
    Serial.println();
 
    initIO();
    wifiConnect();
    mqttConnect();
 
    httpUpdater.setup(&httpServer);
    httpServer.begin();
 
    Serial.println(" Done");
    lastTime = millis();
}
 
void loop() {
    mqtt.loop();
    httpServer.handleClient();
 
    if (millis() - lastTime > 1000) {
        mqtt.publish(ipTopic, WiFi.localIP().toString());
        mqtt.publish(tempTopic, String(getTemp()));
        mqtt.publish(lightTopic, String(analogRead(ADC)));
        mqtt.publish(presenceTopic, presenceDetectee ? "Detectee" : "Non detectee");
 
        Serial.println(analogRead(ADC));
 
        char RSSIstr[5];
        String(WiFi.RSSI()).toCharArray(RSSIstr, 5);
        mqtt.publish(rssiTopic, RSSIstr);
        lastTime = millis();
    }
 
    if(millis()-startTime > 60 * 60 * 1000){
        Serial.println("Restart after 1h");
        ESP.restart();
    }
 
    if(lastPresence != presenceDetectee){
        mqtt.publish(presenceTopic, presenceDetectee ? "Detectee" : "Non detectee");
        digitalWrite(led, presenceDetectee);
        lastPresence = presenceDetectee;
    }
 
    if (!mqtt.connected()) {
        mqttConnect();
    }
}
 
void messageReceived(String inTopic, String payload, char * bytes,
        unsigned int length) {
}
