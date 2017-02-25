/*
 * Demo sketch to send Dallas temperature using an ESP module with locally attached sensor to pin 12
 * 
 * No support, just a demo!
 * 
 * Make sure to fill in your ssid and WiFi password below for ssid & pass.
 * And change the IP configuration.
 * Demo sketch was tested with Domoticz MySensor LAN gateway.
 * 
 * 
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 *  
 * Copyright (C) 2013-2015 Sensnology AB
 * Full contributor list: https://github.com/mysensors/Arduino/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 */

#include <EEPROM.h>
#include <SPI.h>
#include <DallasTemperature.h>
#include <OneWire.h>

// Enable debug prints to serial monitor
#define MY_DEBUG 

// Use a bit lower baudrate for serial prints on ESP8266 than default in MyConfig.h
#define MY_BAUD_RATE 9600

// Gateway mode always enabled for ESP8266. But we add this anyway ;)
#define MY_GATEWAY_ESP8266
#define MY_ESP8266_SSID "maison"
#define MY_ESP8266_PASSWORD "b5c84f1cec"

// Enable MY_IP_ADDRESS here if you want a static ip address (no DHCP)
#define MY_IP_ADDRESS 192,168,0,9
#define MY_IP_GATEWAY_ADDRESS 192,168,0,1
#define MY_IP_SUBNET_ADDRESS 255,255,255,0

// The port to keep open on node server mode 
#define MY_PORT 5003      

// How many clients should be able to connect to this gateway (default 1)
#define MY_GATEWAY_MAX_CLIENTS 2

#include <ESP8266WiFi.h>
#include <MySensor.h>

// Dallas stuff
#define COMPARE_TEMP 1 // Send temperature only if changed? 1 = Yes 0 = No
#define ONE_WIRE_BUS 12 // Pin where dallase sensor is connected 
#define MAX_ATTACHED_DS18B20 16
unsigned long SLEEP_TIME = 30000; // Sleep time between reads (in milliseconds)
OneWire oneWire(ONE_WIRE_BUS); // Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
DallasTemperature sensors(&oneWire); // Pass the oneWire reference to Dallas Temperature. 
float lastTemperature[MAX_ATTACHED_DS18B20];
int numSensors=0;
boolean receivedConfig = false;
boolean metric = true; 
// Initialize temperature message
MyMessage msg(0,V_TEMP);


void setup()  
{ 
  Serial.println("debug setup");
  // Startup up the OneWire library
  sensors.begin();
  // requestTemperatures() will not block current thread
  sensors.setWaitForConversion(false);
}

void presentation() {
  Serial.println("Presentation:");

  // Send the sketch version information to the gateway and Controller
  sendSketchInfo("Temperature Sensor", "1.1");

  // Fetch the number of attached temperature sensors  
  numSensors = sensors.getDeviceCount();
  Serial.print("sensors: ");
  Serial.println(numSensors);

  // Present all sensors to controller
  for (int i=0; i<numSensors && i<MAX_ATTACHED_DS18B20; i++) {   
     present(i, S_TEMP);
  }
}

void loop()     
{     
  Serial.println("Loop:");
  // Fetch temperatures from Dallas sensors
  sensors.requestTemperatures();

  // query conversion time and sleep until conversion completed
  int16_t conversionTime = sensors.millisToWaitForConversion(sensors.getResolution());
  // sleep() call can be replaced by wait() call if node need to process incoming messages (or if node is repeater)
  sleep(conversionTime);

  // Read temperatures and send them to controller 
  for (int i=0; i<numSensors && i<MAX_ATTACHED_DS18B20; i++) {
 
    // Fetch and round temperature to one decimal
    float temperature = static_cast<float>(static_cast<int>((getConfig().isMetric?sensors.getTempCByIndex(i):sensors.getTempFByIndex(i)) * 10.)) / 10.;

    Serial.print("Reading sensor: ");
    Serial.print(i);
    Serial.print(" with value: ");
    Serial.println(temperature);
 
    // Only send data if temperature has changed and no error
    #if COMPARE_TEMP == 1
    if (lastTemperature[i] != temperature && temperature != -127.00 && temperature != 85.00) {
    #else
    if (temperature != -127.00 && temperature != 85.00) {
    #endif
 
      // Send in the new temperature
      send(msg.setSensor(i).set(temperature,1));
      // Save new temperatures for next compare
      lastTemperature[i]=temperature;
    }
  }
  delay(5000);
}
