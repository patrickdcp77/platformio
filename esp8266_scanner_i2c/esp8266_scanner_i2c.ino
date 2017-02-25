#include <JeeLib.h>
#include <RF69_compat.h>
#include <RF69_avr.h>
#include <PortsBMP085.h>
#include <Ports.h>
#include <RF12.h>
#include <RF12sio.h>
#include <RF69.h>
#include <PortsLCD.h>
#include <PortsSHT11.h>

// --------------------------------------
// Jeenode_i2c_scanner
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    http://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
// Version 6, March 21, 2014, Using Arduino 1.0.5 / current JeeLib from git
//    by Gareth Coleman, l0l.org.uk.
//    Reworked scanner so that it uses JeeLib instead of Wire and can also
//    scan I2C devices on 'ports' on JeeNodes. SDA goes to DIO, SCK to AIO.
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//
#include <JeeLib.h>

byte presence, address, port;
int nDevices;

void setup()
{
  Serial.begin(9600);
  Serial.println("\nI2C Scanner");
}

void loop()
{
  nDevices = 0;

  Serial.println("Scanning...");

  for(port = 0; port <= 4; port++ )     // Port 0 means (hardware) I2C interface, ports 1-4 use software
  {
    for(address = 1; address < 127; address++ ) 
    {
      // This i2c_scanner uses the return value of
      // the DeviceI2C.isPresent function to see if
      // a device exists. For overview of the PortI2C
      // and DeviceI2C functions see
      // http://jeelabs.org/2012/03/21/porti2c-the-big-picture/

      presence = DeviceI2C(PortI2C(port),address).isPresent();

      if (presence != 0)
      {
        Serial.print("I2C device found on ");
        if (port==0)
          Serial.print("the hardware I2C interface");
        else
        {
          Serial.print("port ");
          Serial.print(port);
        }
        Serial.print(" at address 0x");
        if (address<16) 
          Serial.print("0");
        Serial.print(address,HEX);
        Serial.println("  !");

        nDevices++;
      }
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000);           // wait 5 seconds for next scan
}
