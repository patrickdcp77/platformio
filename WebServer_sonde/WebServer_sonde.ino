/*
  Web Server

 A simple web server that shows the value of the analog input pins.
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
*/








#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

/* This driver uses the Adafruit unified sensor library (Adafruit_Sensor),
   which provides a common 'type' for sensor data and some helper functions.
   
   To use this driver you will also need to download the Adafruit_Sensor
   library and include it in your libraries folder.

   You should also assign a unique ID to this sensor for use with
   the Adafruit Sensor API so that you can identify this particular
   sensor in any data logs, etc.  To assign a unique ID, simply
   provide an appropriate value in the constructor below (12345
   is used by default in this example).
   
   Connections
   ===========
   Connect SCL to analog 5
   Connect SDA to analog 4
   Connect VDD to 3.3V DC
   Connect GROUND to common ground
    
   History
   =======
   2013/JUN/17  - Updated altitude calculations (KTOWN)
   2013/FEB/13  - First version (KTOWN)
*/
   
Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);

/**************************************************************************/
/*
    Displays some basic information on this sensor from the unified
    sensor API sensor_t type (see Adafruit_Sensor for more information)
*/
/**************************************************************************/


/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {

  //0xE8, 0x4E, 0x06, 0s19, 0sC2, 0s8F
  0x00, 0xD0, 0xBE, 0xEF, 0xFE, 0xD8
};
IPAddress ip(192, 168, 0, 16);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

//EthernetClient client = server.available();


void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
//******************************
//initialisation led
pinMode(2, OUTPUT);
pinMode(3, OUTPUT);
pinMode(5, OUTPUT);
pinMode(6, OUTPUT);
pinMode(7, OUTPUT);
pinMode(8, OUTPUT);
pinMode(9, OUTPUT);


//*********************************************************
//initialisation sonde

Serial.println("Pressure Sensor Test"); Serial.println("");
  
  /* Initialise the sensor */
  if(!bmp.begin())
  {
    /* There was a problem detecting the BMP085 ... check your connections */
    Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  /* Display some basic information on this sensor */
  displaySensorDetails();
//***********************************************************

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}

//**************************************
//boucle principale

void loop() {

  
//**********************************
//la led

//*************************************
  
  // listen for incoming clients
  EthernetClient client = server.available();//************************************
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          for (int analogChannel = 0; analogChannel < 1; analogChannel++) {  //on balaye A0 seulement sinon analogChannel < 6  pour 0 à 5
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);

            client.print ("      led clignote");

//*appel led
led();

//****************************************
//appel calcul sonde

//sonde();

 /* Get a new sensor event */ 
  sensors_event_t event;
  bmp.getEvent(&event);



  /* First we get the current temperature from the BMP085 */
    float temperature;
    bmp.getTemperature(&temperature);
   

client.print("  Temperature: ");
    client.print(temperature);
    client.println(" C");

    /* Then convert the atmospheric pressure, and SLP to altitude         */
    /* Update this next line with the current SLP for better results      */
    float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;


  
client.print(" Altitude:    "); 
    client.print(bmp.pressureToAltitude(seaLevelPressure,
                                        event.pressure)); 
    client.println(" m");
    client.println("");

  /* Display atmospheric pressue in hPa */
    client.print("  Pressure:    ");
    client.print(event.pressure);
    client.println(" hPa");
    
//******************************************


           
            client.println("<br />");



            
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

//**********************************
//led clignote

void led(){


 digitalWrite(2, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
   digitalWrite(3, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
   digitalWrite(5, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
   digitalWrite(6, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
   digitalWrite(7, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
   digitalWrite(8, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);              // wait for a second
   digitalWrite(9, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500); 
  // wait for a second

  
  digitalWrite(9, LOW);    // turn the LED off by making the voltage LOW
  delay(500);              // wait for a second
  digitalWrite(8, LOW);    // turn the LED off by mdigitalWrite(7, LOW);    // turn the LED off by making the voltage LOW
  delay(500);              // wait for a secondaking the voltage LOW
  delay(500);              // wait for a second
  digitalWrite(7, LOW);    // turn the LED off by making the voltage LOW
  delay(500);              // wait for a second
  digitalWrite(6, LOW);    // turn the LED off by making the voltage LOW
  delay(500);              // wait for a second
  digitalWrite(5, LOW);    // turn the LED off by making the voltage LOW
  delay(500);              // wait for a second
  digitalWrite(3, LOW);    // turn the LED off by making the voltage LOW
  delay(500);   
  digitalWrite(2, LOW);    // turn the LED off by making the voltage LOW
  delay(500);              // wait for a second
  // wait for a second
  
}
//************************************************

void sonde() 
{

EthernetClient client = server.available();//***********************************


  
  /* Get a new sensor event */ 
  sensors_event_t event;
  bmp.getEvent(&event);
 
  /* Display the results (barometric pressure is measure in hPa) */
  if (event.pressure)
  {
    /* Display atmospheric pressue in hPa */
    Serial.print("Pressure:    ");
    Serial.print(event.pressure);
    Serial.println(" hPa");
    
    /* Calculating altitude with reasonable accuracy requires pressure    *
     * sea level pressure for your position at the moment the data is     *
     * converted, as well as the ambient temperature in degress           *
     * celcius.  If you don't have these values, a 'generic' value of     *
     * 1013.25 hPa can be used (defined as SENSORS_PRESSURE_SEALEVELHPA   *
     * in sensors.h), but this isn't ideal and will give variable         *
     * results from one day to the next.                                  *
     *                                                                    *
     * You can usually find the current SLP value by looking at weather   *
     * websites or from environmental information centers near any major  *
     * airport.                                                           *
     *                                                                    *
     * For example, for Paris, France you can check the current mean      *
     * pressure and sea level at: http://bit.ly/16Au8ol                   */
     
    /* First we get the current temperature from the BMP085 */
    float temperature;
    bmp.getTemperature(&temperature);
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

client.print("Temperature: ");
    client.print(temperature);
    client.println(" C");

    

    /* Then convert the atmospheric pressure, and SLP to altitude         */
    /* Update this next line with the current SLP for better results      */
    float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
    Serial.print("Altitude:    "); 
    Serial.print(bmp.pressureToAltitude(seaLevelPressure,
                                        event.pressure)); 
    Serial.println(" m");
    Serial.println("");


client.print("Altitude:    "); 
    client.print(bmp.pressureToAltitude(seaLevelPressure,
                                        event.pressure)); 
    client.println(" m");
    client.println("");
    
  }
  else
  {
    Serial.println("Sensor error");
  }
  delay(1000);





  
}


void displaySensorDetails(void)
{
  sensor_t sensor;
  bmp.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" hPa");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" hPa");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" hPa");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}
