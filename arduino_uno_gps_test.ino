/*
  ========================================================================
  Arduino Uno - GPS Hardware & Satellite Test
  ========================================================================
  Wiring Diagram:
  - GPS Module VCC  -->  Arduino Uno 5V (or 3.3V)
  - GPS Module GND  -->  Arduino Uno GND
  - GPS Module TX   -->  Arduino Uno Pin 8 (SoftwareSerial RX)
  - GPS Module RX   -->  Arduino Uno Pin 9 (SoftwareSerial TX) [Optional]

  Required Library:
  - TinyGPSPlus by Mikal Hart (Install via Arduino Library Manager)
  ========================================================================
*/

#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// SoftwareSerial Pin Definitions for Arduino Uno
static const int RXPin = 8; // Arduino Uno Pin 8 connects to GPS TX
static const int TXPin = 9; // Arduino Uno Pin 9 connects to GPS RX
static const uint32_t GPS_BAUD = 9600;

SoftwareSerial gpsSerial(RXPin, TXPin);
TinyGPSPlus gps;

unsigned long lastCharReceivedTime = 0;
unsigned long lastDisplayTime = 0;
unsigned long totalCharsReceived = 0;

void setup() {
  // Serial Monitor Baud Rate (set your Serial Monitor dropdown to 9600)
  Serial.begin(9600);
  
  // Software Serial communication with GPS
  gpsSerial.begin(GPS_BAUD);

  Serial.println(F("=================================================="));
  Serial.println(F("       Arduino Uno - GPS Module Test             "));
  Serial.println(F("=================================================="));
  Serial.println(F("Starting GPS diagnostics...\n"));
}

void loop() {
  // 1. Continuously feed incoming GPS characters into TinyGPS++
  while (gpsSerial.available() > 0) {
    char c = gpsSerial.read();
    gps.encode(c);
    lastCharReceivedTime = millis();
    totalCharsReceived++;
  }

  // 2. Refresh status display once every 1.5 seconds
  if (millis() - lastDisplayTime >= 1500) {
    lastDisplayTime = millis();

    // Determine Hardware status
    // The GPS transmits NMEA sentences continuously once powered on.
    // If no bytes received within 3 seconds, hardware communication is broken.
    bool hardwareOk = (totalCharsReceived > 0) && (millis() - lastCharReceivedTime < 3000);

    Serial.println(F("=================================================="));
    
    if (!hardwareOk) {
      // ----------------------------------------------------------------
      // Hardware NOT OK
      // ----------------------------------------------------------------
      Serial.println(F("Hardware Status : NOT OK"));
      Serial.println(F("Reason          : No data received from GPS module."));
      Serial.println(F("Check:"));
      Serial.println(F("  1. GPS TX wire -> Arduino Uno Pin 4."));
      Serial.println(F("  2. GPS VCC -> 5V (or 3.3V) and GND -> GND."));
      Serial.println(F("  3. Ensure GPS module power LED is ON."));
    } 
    else {
      // ----------------------------------------------------------------
      // Hardware OK
      // ----------------------------------------------------------------
      Serial.println(F("Hardware Status : OK"));

      // Check if valid satellite fix & location data is available
      if (gps.location.isValid() && gps.location.age() < 3000) {
        Serial.println(F("GPS Data        : AVAILABLE (3D Fix Acquired!)"));
        Serial.println(F("--------------------------------------------------"));
        
        Serial.print(F("Latitude        : "));
        Serial.println(gps.location.lat(), 6);

        Serial.print(F("Longitude       : "));
        Serial.println(gps.location.lng(), 6);

        Serial.print(F("Satellites      : "));
        if (gps.satellites.isValid()) {
          Serial.println(gps.satellites.value());
        } else {
          Serial.println(F("N/A"));
        }

        Serial.print(F("Altitude        : "));
        if (gps.altitude.isValid()) {
          Serial.print(gps.altitude.meters(), 1);
          Serial.println(F(" m"));
        } else {
          Serial.println(F("N/A"));
        }
      } 
      else {
        // Satellite searching / Waiting for fix
        Serial.println(F("GPS Data        : Searching for satellites..."));
        
        Serial.print(F("Satellites Visible : "));
        if (gps.satellites.isValid()) {
          Serial.println(gps.satellites.value());
        } else {
          Serial.println(F("Searching (0 visible)"));
        }
        
        Serial.println(F("Note: GPS fix requires open sky outdoors or near a window."));
        Serial.println(F("      First fix can take 1-3 minutes."));
      }
    }
    Serial.println(F("==================================================\n"));
  }
}
