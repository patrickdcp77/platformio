#include <Arduino.h>
#include <stdlib.h>
#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include <ArduinoOTA.h>
#include <OneWire.h>
#include <DallasTemperature.h>
 
//#include <config.h>
 
WiFiClient net;
MQTTClient mqtt;
 
OneWire oneWire(tempPin);
DallasTemperature sensors(&oneWire);
 
int state = 0;
unsigned long lastTime;
 
float R;
const int pwmIntervals = 100;
 
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
 
    WiFi.begin(ssid, pass);
 
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
    attachInterrupt(presence, presenceNonDetecteeInterrupt, FALLING);
}
 
void presenceNonDetecteeInterrupt(){
    Serial.println("Presence Non detectee");
    presenceDetectee = false;
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
    char tempString[6];
    char vBatStr[5];
 
    Serial.begin(115200);
    Serial.println();
 
    initIO();
    wifiConnect();
    mqttConnect();
 
    ArduinoOTA.begin();
 
    Serial.println(" Done");
    lastTime = millis();
}
 
void loop() {
    char tempString[6];
    mqtt.loop();
    ArduinoOTA.handle();
 
    if (millis() - lastTime > 1000) {
        float t = getTemp();
        dtostrf(t,6,3,tempString);
 
        mqtt.publish(tempTopic,     tempString);
 
        mqtt.publish(lightTopic, String(analogRead(ADC)));
        Serial.println(analogRead(ADC));
 
        char RSSIstr[5];
        String(WiFi.RSSI()).toCharArray(RSSIstr, 5);
        mqtt.publish(rssiTopic, RSSIstr);
        lastTime = millis();
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
