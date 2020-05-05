// Import required libraries
#include <Servo.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <Wire.h>
#include <WiFiClient.h>
#define addr 0x1E //I2C Address for The HMC5883

// Replace with the wanted network credentials

#ifndef APSSID
#define APSSID "ESPap"
#define APPSK  "mdrtkisalo"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

// Set LED GPIO
const int ledPin = 2;
// Stores LED state
String ledState;

Servo myservo; // create servo object to control a servo
int angle = 0; 

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);



String getX() {

}

// Replaces placeholder with LED state value
String processor(const String& var){
  Serial.println(var);
  if (var == "TEMPERATURE"){
    return getX();
  }
}

void setup() {

  Wire.begin();
  
 myservo.attach(D4);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  server.begin();
  Serial.println("HTTP server started");

  // Initialize SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  Wire.beginTransmission(addr); //start talking
  Wire.write(0x02); // Set the Register
  Wire.write(0x00); // Tell the HMC5883 to Continuously Measure
  Wire.endTransmission();

 
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false);
  });

  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });

 // Route to set GPIO to HIGH
  server.on("/plus90", HTTP_GET, [](AsyncWebServerRequest *request){
 myservo.write(0);  
    request->send(SPIFFS, "/index.html", String(), false);
  });
  
  // Route to set GPIO to LOW
  server.on("/minus90", HTTP_GET, [](AsyncWebServerRequest *request){
    myservo.write(255);  
    request->send(SPIFFS, "/index.html", String(), false);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", getX().c_str());
  });

  // Start server
  server.begin();
}

void loop() {

}
