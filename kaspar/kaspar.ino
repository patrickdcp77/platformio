void setup() {

    // Setup computer to Arduino serial
    Serial.begin(9600);

    // Setup Arduino to ESP8266 serial
    // Use baud rate 115200 during firmware update
    Serial1.begin(9600);

}

void loop() {

    // Send bytes from ESP8266 to computer
    if ( Serial1.available() ) {
        Serial.write( Serial1.read() );
    }

    // Send bytes from computer back to ESP8266
    if ( Serial.available() ) {
        Serial1.write( Serial.read() );
    }

}
