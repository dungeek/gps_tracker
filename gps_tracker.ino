#include <TinyGPSPlus.h>
#include <WiFi.h>
#include <Blynk.h>
#include <>

#define BLYNK_TEMPLATE_ID "TMPL6ocSuuDff"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "0P-tfXHEI9IVW0AdJSms7Q98KkA8gei7"

// The TinyGPSPlus object
TinyGPSPlus gps;

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  delay(3000);
}

void loop() {
  //updateSerial();

  while (Serial2.available() > 0)

    if (gps.encode(Serial2.read()))

      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)

  {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }
}

void displayInfo() {
  Serial.println(F("Location: "));
 
  if (gps.location.isValid()) {
    Serial.print(F("Latitude: "));
    Serial.println(gps.location.lat(), 6);
    Serial.print(F("Longitude: "));
    Serial.println(gps.location.lng(), 6);
  } else {
    Serial.println(F("INVALID"));
  }
  Serial.println();  
}

void updateSerial()
{
  delay(500);
  
  while (Serial.available())
  {
    Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port
  }

  while (Serial2.available())
  {
    Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port
  }

}
