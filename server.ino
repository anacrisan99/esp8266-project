#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"


// Set your access point network credentials
const char* ssid = "ESP8266";
const char* password = "123456789";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String mesaj1() {
  return String("Acesta este un mesaj trimis de la server la client");
}

String mesaj2() {
  return String("Proiect realizat de:   Ana Crisan, 333AB");
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(9600);
  Serial.println();
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  server.on("/mesaj1", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", mesaj1().c_str());
  });
  server.on("/mesaj2", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", mesaj2().c_str());
  });
  
  bool status;
  // Start server
  server.begin();
}
 
void loop(){
  
}
