#define BLYNK_TEMPLATE_ID "TMPL6eMIZya0d"
#define BLYNK_TEMPLATE_NAME "GPS Tracker"
#define BLYNK_AUTH_TOKEN "5UDry8pEa2meGmZJdH-w69xeWmAlqOL4"

#define BLYNK_PRINT Serial

#include <TinyGPSPlus.h>
#include <BlynkSimpleEsp32.h>

// The TinyGPSPlus object is used to store and parse GPS data.
TinyGPSPlus gps;

// Wifi credentials
const char *ssid =  "Galaxy M328DCB";  // Name of your network 
const char *pass =  "12345678"; //Password

BlynkTimer timer;
float latitude;     //Storing the Latitude
float longitude;    //Storing the Longitude
float spd;       //Variable  to store the speed
float sats;      //Variable to store no. of satellites response
String bearing;  //Variable to store orientation or direction of GPS


void setup() {
  Serial.begin(9600);   // Initialize the serial communication 
  Serial2.begin(9600); // Initialize serial communication with GPS module
  Serial.println();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(5000L, checkGPS); // Every 5s check if GPS is connected
}

void loop() {
  while (Serial2.available() > 0)
  {
    if (gps.encode(Serial2.read())){
      displayInfo();
    }
  }

  Blynk.run();
  timer.run();
}

void displayInfo() {
  Serial.println(F("Location: "));
  if (gps.location.isValid()) 
  {
    latitude = (gps.location.lat()); //Storing the Lat. and Lon. 
    longitude = (gps.location.lng());

    Serial.print(F("Latitude: "));
    Serial.println(gps.location.lat(), 6);
    Serial.print(F("Longitude: "));
    Serial.println(gps.location.lng(), 6);

    Blynk.virtualWrite(V1, String(latitude, 6));   
    Blynk.virtualWrite(V2, String(longitude, 6));  

  //   Serial.print("Speed: ");
  //   Serial.println(spd);
  //   sats = gps.satellites.value();    //get number of satellites
  //   Blynk.virtualWrite(V3, sats);

  //   bearing = TinyGPSPlus::cardinal(gps.course.value()); // get the direction
  //   Blynk.virtualWrite(V4, bearing); 
  } else {
    Serial.println(F("INVALID"));
    Blynk.virtualWrite(V5, "Invalid data");
  }
  Serial.println();  // Add a newline for better readability
}

// If no valid GPS data is received after 5 seconds and the number of characters processed by TinyGPSPlus is less than 10.
void checkGPS(){
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    Blynk.virtualWrite(V4, "GPS ERROR");  
  }
}